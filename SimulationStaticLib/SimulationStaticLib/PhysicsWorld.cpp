#include "pch.h"
#include "PhysicsWorld.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include <algorithm>

void PhysicsWorld::addObject(PhysicsObject* object)
{
	if (object == nullptr)
	{
		return;
	}
	_objects.push_back(object);
}

void PhysicsWorld::removeObject(PhysicsObject* object)
{
	_objects.erase(std::remove(_objects.begin(), _objects.end(), object), _objects.end());
}

void PhysicsWorld::addSpring(const Spring& spring)
{
	_springs.push_back(new Spring(spring));
}

void PhysicsWorld::step(float deltaTime)
{
	applyGlobalForces(deltaTime);
	handleCollisions(deltaTime);
	integrate(deltaTime);
}

void PhysicsWorld::applyGlobalForces(float deltaTime)
{
	for (auto& objects : _objects)
	{
		objects->addForce(_gravity * objects->getMass());
	}
}

void PhysicsWorld::integrate(float deltaTime)
{
	for (auto& objects : _objects)
	{
		switch (_selectedIntegrationMethod)
		{
		case IntegrationMethod::Euler:
			objects->IntegrateEuler(deltaTime);
			break;
		case IntegrationMethod::SemiImplicitEuler:
			objects->IntegrateSemiImplicitEuler(deltaTime);
			break;
		default:
			break;
		}
	}
}

void PhysicsWorld::setIntegrationMethod(IntegrationMethod method)
{
	_selectedIntegrationMethod = method;
}

void PhysicsWorld::resolveCollision(PhysicsObject* objectA, PhysicsObject* objectB, const CollisionEvent& collisionEvent, float deltaTime)
{
	RigidBody* a = dynamic_cast<RigidBody*>(objectA);
	RigidBody* b = dynamic_cast<RigidBody*>(objectB);

	if (a == nullptr || b == nullptr) {
		return;
	}

	const bool aStatic = a->isStatic();
	const bool bStatic = b->isStatic();

	if (aStatic && bStatic) {
		return;
	}

	glm::vec3 n = collisionEvent.collisionNormal;
	if (glm::dot(n, n) < 1e-8f) {
		n = b->getPos() - a->getPos();
	}

	if (glm::dot(n, n) < 1e-8f) {
		n = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else {
		n = glm::normalize(n);
	}

	const glm::vec3 aToB = b->getPos() - a->getPos();
	if (glm::dot(n, aToB) < 0.0f) {
		n = -n;
	}

	const float invMassA = aStatic ? 0.0f : a->getInverseMass();
	const float invMassB = bStatic ? 0.0f : b->getInverseMass();
	const float invMassSum = invMassA + invMassB;

	if (invMassSum <= 0.0f) {
		return;
	}

	// Use contact point for angular-aware relative velocity
	glm::vec3 contactPoint = collisionEvent.collisionPoint;
	if (glm::dot(contactPoint, contactPoint) < 1e-12f) {
		contactPoint = 0.5f * (a->getPos() + b->getPos());
	}

	const glm::vec3 ra = contactPoint - a->getPos();
	const glm::vec3 rb = contactPoint - b->getPos();

	const glm::vec3 va = a->getVelocity() + glm::cross(a->getAngularVelocity(), ra);
	const glm::vec3 vb = b->getVelocity() + glm::cross(b->getAngularVelocity(), rb);

	const glm::vec3 relativeVelocity = vb - va;
	const float velocityAlongNormal = glm::dot(relativeVelocity, n);

	if (velocityAlongNormal < 0.0f) {
		const float restitution = 0.35f;
		const float j = -(1.0f + restitution) * velocityAlongNormal / invMassSum;
		const glm::vec3 impulse = j * n;

		const float safeDt = std::max(deltaTime, 1e-6f);
		const glm::vec3 force = impulse / safeDt;

		if (!aStatic) {
			a->addForceAtPoint(-force, contactPoint);
		}
		if (!bStatic) {
			b->addForceAtPoint(force, contactPoint);
		}
	}

	// Positional correction
	const float slop = 0.001f;
	const float percent = 0.9f;
	const float penetration = std::max(collisionEvent.penetrationDepth - slop, 0.0f);
	const glm::vec3 correction = (penetration / invMassSum) * percent * n;

	if (!aStatic) {
		a->setPos(a->getPos() - correction * invMassA);
	}
	if (!bStatic) {
		b->setPos(b->getPos() + correction * invMassB);
	}
}

void PhysicsWorld::handleCollisions(float deltaTime)
{
	for (PhysicsObject* object : _objects) {
		if (object == nullptr || object->GetCollider() == nullptr) {
			continue;
		}

		RigidBody* rigidBody = dynamic_cast<RigidBody*>(object);
		if (rigidBody != nullptr) {
			Transform colliderTransform = object->GetCollider()->GetTransform();
			colliderTransform.setPosition(rigidBody->getPos());
			colliderTransform.setRotation(rigidBody->getOrientation());
			object->GetCollider()->SetTransform(colliderTransform);
		}
	}

	for (size_t i = 0; i < _objects.size(); ++i) {
		PhysicsObject* aObject = _objects[i];
		if (aObject == nullptr || aObject->GetCollider() == nullptr) {
			continue;
		}

		for (size_t j = i + 1; j < _objects.size(); ++j) {
			PhysicsObject* bObject = _objects[j];
			if (bObject == nullptr || bObject->GetCollider() == nullptr) {
				continue;
			}

			Collider* aCollider = aObject->GetCollider();
			Collider* bCollider = bObject->GetCollider();
			//if (aCollider->IsContainer() || bCollider->IsContainer()) {
			//	continue;
			//}

			CollisionEvent event{};
			if (!aObject->GetCollider()->Collide(*bObject->GetCollider(), event)) {
				continue;
			}

			if (!event.isColliding) {
				continue;
			}

			resolveCollision(aObject, bObject, event, deltaTime);
		}
	}
}

void PhysicsWorld::setGravity(const glm::vec3& gravity)
{
	_gravity = gravity;
}


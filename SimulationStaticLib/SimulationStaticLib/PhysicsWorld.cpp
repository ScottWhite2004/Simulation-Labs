#include "pch.h"
#include "PhysicsWorld.h"

void PhysicsWorld::addObject(const PhysicsObject& object)
{
	_objects.push_back(new PhysicsObject(object));
}

void PhysicsWorld::addSpring(const Spring& spring)
{
	_springs.push_back(new Spring(spring));
}

void PhysicsWorld::step(float deltaTime)
{
	applyGlobalForces(deltaTime);
	integrate(deltaTime);
	handleCollisions();
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

void PhysicsWorld::handleCollisions()
{
	for (auto i = 0; i < _objects.size(); i++)
	{
		for (auto j = 0; j < _objects.size(); j++)
		{
			//Check for collisions between _rigidBodies[i] and _rigidBodies[j]
			//If collision detected, create CollisionEvent and resolve collision
		}
	}
}

void PhysicsWorld::setGravity(const glm::vec3& gravity)
{
	_gravity = gravity;
}

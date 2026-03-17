#include "pch.h"
#include "PhysicsObject.h"

PhysicsObject::PhysicsObject() = default;

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::IntegrateEuler(float seconds)
{
	glm::vec3 acceleration = _gravity * _inverseMass; // Assuming only gravity for this scenario
	_position += _velocity * seconds;
	_velocity += acceleration * seconds;
	_sphereCollider.SetPosition(_position);
}

void PhysicsObject::IntegrateEuler(float seconds, PhysicsObject& otherBody)
{
	IntegrateEuler(seconds);
	if (_sphereCollider.CollidesWith(otherBody.GetSphere()))
	{
		ResolveSphereSphereCollision(otherBody);
	}
}

void PhysicsObject::IntegrateEuler(float seconds, const Plane& staticPlane)
{
	IntegrateEuler(seconds);
	if(_sphereCollider.CollidesWith(staticPlane))
	{
		_velocity = glm::vec3(0.0f); // Simple collision response: zero velocity
	}
}

void PhysicsObject::IntegrateSemiImplicitEuler(float seconds)
{
	glm::vec3 acceleration = _gravity * _inverseMass; // Assuming only gravity for this scenario
	_velocity += acceleration * seconds;
	_position += _velocity * seconds;
	_sphereCollider.SetPosition(_position);
}

void PhysicsObject::IntegrateSemiImplicitEuler(float seconds, PhysicsObject& otherBody)
{
	IntegrateSemiImplicitEuler(seconds);
	if (_sphereCollider.CollidesWith(otherBody.GetSphere()))
	{
		ResolveSphereSphereCollision(otherBody);
	}
}

void PhysicsObject::IntegrateSemiImplicitEuler(float seconds, const Plane& staticPlane)
{
	IntegrateSemiImplicitEuler(seconds);
	if(_sphereCollider.CollidesWith(staticPlane))
	{
		_velocity = glm::vec3(0.0f); // Simple collision response: zero velocity
	}
}

void PhysicsObject::ResolveSphereSphereCollision(PhysicsObject& other)
{
	float distance = glm::length(other.getPos() - _position);
	glm::vec3 normal = glm::normalize(other.getPos() - _position);
	if (distance == 0.0f)
	{
		normal = glm::vec3(1.0f, 0.0f, 0.0f); // Arbitrary normal if spheres are at the same position
	}
	glm::vec3 relativeVelocity = _velocity - other.getVel();
	float velAlongNormal = glm::dot(relativeVelocity, normal);

	if (velAlongNormal <= 0.0f)
		return;

	float e = 1.0f; // coefficient of restitution

	float m1 = _mass;
	float m2 = other.getMass();

	float j = -(1.0f + e) * velAlongNormal;
	j /= (1.0f / m1 + 1.0f / m2);

	glm::vec3 impulse = j * normal;

	_velocity += impulse / m1;
	other.SetVelocity(other.getVel() - impulse / m2);
}

void PhysicsObject::addAngularDisplacement(const glm::vec3& axis, const float& radians)
{
	glm::vec3 normalizedAxis = glm::normalize(axis);
	float x = glm::cos(radians / 2.0f);
	float y = normalizedAxis.x * glm::sin(radians / 2.0f);
	float z = normalizedAxis.y * glm::sin(radians / 2.0f);
	float w = normalizedAxis.z * glm::sin(radians / 2.0f);
	glm::quat angularDisplacement = glm::quat(x, y, z, w);
	addAngularDisplacement(angularDisplacement);
}

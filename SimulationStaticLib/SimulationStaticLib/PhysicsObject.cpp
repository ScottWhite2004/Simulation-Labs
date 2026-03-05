#include "pch.h"
#include "PhysicsObject.h"

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::IntegrateEuler(float seconds)
{
	glm::vec3 acceleration = _gravity * _inverseMass; // Assuming only gravity for this scenario
	_position += _velocity * seconds;
	_velocity += acceleration * seconds;
	_sphere.SetPosition(_position);
}

void PhysicsObject::IntegrateEuler(float seconds, PhysicsObject& otherBody)
{
	IntegrateEuler(seconds);
	if (_sphere.CollidesWith(otherBody.GetSphere()))
	{
		ResolveSphereSphereCollision(otherBody);
	}
}

void PhysicsObject::IntegrateEuler(float seconds, const Plane& staticPlane)
{
	IntegrateEuler(seconds);
	if(_sphere.CollidesWith(staticPlane))
	{
		_velocity = glm::vec3(0.0f); // Simple collision response: zero velocity
	}
}

void PhysicsObject::IntegrateSemiImplicitEuler(float seconds)
{
	glm::vec3 acceleration = _gravity * _inverseMass; // Assuming only gravity for this scenario
	_velocity += acceleration * seconds;
	_position += _velocity * seconds;
	_sphere.SetPosition(_position);
}

void PhysicsObject::IntegrateSemiImplicitEuler(float seconds, PhysicsObject& otherBody)
{
	IntegrateSemiImplicitEuler(seconds);
	if (_sphere.CollidesWith(otherBody.GetSphere()))
	{
		ResolveSphereSphereCollision(otherBody);
	}
}

void PhysicsObject::IntegrateSemiImplicitEuler(float seconds, const Plane& staticPlane)
{
	IntegrateSemiImplicitEuler(seconds);
	if(_sphere.CollidesWith(staticPlane))
	{
		_velocity = glm::vec3(0.0f); // Simple collision response: zero velocity
	}
}

void PhysicsObject::ResolveSphereSphereCollision(PhysicsObject& other)
{
	// Compute collision normal from this to other
	glm::vec3 normal = other.getPos() - _position;
	float distance = glm::length(normal);
	if (distance == 0.0f)
	{
		// Degenerate: centres coincide, pick an arbitrary normal
		normal = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		normal /= distance;
	}

	// Relative velocity along normal
	const glm::vec3 v1 = _velocity;
	const glm::vec3 v2 = other.getVel();

	glm::vec3 relativeVelocity = v1 - v2;
	float relNormalSpeed = glm::dot(relativeVelocity, normal);

	// If spheres are separating or stationary along normal, do nothing
	if (relNormalSpeed <= 0.0f)
	{
		return;
	}

	// Project velocities onto normal
	float u1n = glm::dot(v1, normal);
	float u2n = glm::dot(v2, normal);

	// Tangential components (perpendicular to normal)
	glm::vec3 v1t = v1 - u1n * normal;
	glm::vec3 v2t = v2 - u2n * normal;

	// Masses
	float m1 = _mass;
	float m2 = other.getMass();
	float denom = m1 + m2;
	if (denom == 0.0f)
	{
		// Avoid divide by zero: just bail out
		return;
	}

	// 1D perfectly elastic collision along the normal:
	// v1 = ((m1 - m2)/(m1 + m2)) * u1 + (2 m2/(m1 + m2)) * u2
	// v2 = (2 m1/(m1 + m2)) * u1 + ((m2 - m1)/(m1 + m2)) * u2
	float v1n = ((m1 - m2) / denom) * u1n + (2.0f * m2 / denom) * u2n;
	float v2n = (2.0f * m1 / denom) * u1n + ((m2 - m1) / denom) * u2n;

	// Reconstruct full 3D velocities: tangential + new normal component
	_velocity = v1t + v1n * normal;
	other.SetVelocity(v2t + v2n * normal);
}

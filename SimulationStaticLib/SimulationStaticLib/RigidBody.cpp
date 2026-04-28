#include "pch.h"
#include "RigidBody.h"

RigidBody::RigidBody()
	: _transform(glm::vec3(0.0f), glm::vec3(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
{

}

RigidBody::RigidBody(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& velocity, float mass)
	: _transform(position, glm::vec3(1.0f), glm::quat(rotation)),
	_velocity(velocity)
{
	SetMass(mass);
}

void RigidBody::IntegrateEuler(float seconds)
{
	if (isStatic())
	{
		clearForces();
		return;
	}

	glm::vec3 linearAcceleration = getAccumulatedForces() * getInverseMass();
	glm::vec3 angularAcceleration = _torque * _inverseInertia;

	glm::vec3 newPosition = _transform.getPosition() + (_velocity * seconds);
	_transform.setPosition(newPosition);

	_velocity += linearAcceleration * seconds;

	_angularVelocity += angularAcceleration * seconds;
	if (glm::length(_angularVelocity) > 0.0001f)
	{
		float angle = glm::length(_angularVelocity) * seconds;
		glm::vec3 axis = glm::normalize(_angularVelocity);
		addAngularDisplacement(axis, angle);
		_transform.setRotation(glm::normalize(_transform.getRotation()));
	}

	clearForces();
}

void RigidBody::IntegrateSemiImplicitEuler(float seconds)
{
	if (isStatic())
	{
		clearForces();
		return;
	}
	const glm::vec3 linearAcceleration = getAccumulatedForces() * getInverseMass();
	const glm::vec3 angularAcceleration = _torque * _inverseInertia;

	_velocity += linearAcceleration * seconds;

	glm::vec3 newPosition = _transform.getPosition() + (_velocity * seconds);
	_transform.setPosition(newPosition);

	_angularVelocity += angularAcceleration * seconds;
	if (glm::length(_angularVelocity) > 0.0001f)
	{
		float angle = glm::length(_angularVelocity) * seconds;
		glm::vec3 axis = glm::normalize(_angularVelocity);
		addAngularDisplacement(axis, angle);
		_transform.setRotation(glm::normalize(_transform.getRotation()));
	}
	clearForces();
}

void RigidBody::addAngularDisplacement(const glm::vec3& axis, const float& radians)
{
	glm::quat angularDisplacement = glm::angleAxis(radians, axis);
	addAngularDisplacement(angularDisplacement);
}

void RigidBody::addAngularDisplacement(const glm::quat& angularDisplacement)
{
	_transform.setRotation(angularDisplacement * _transform.getRotation());
}

void RigidBody::addForceAtPoint(const glm::vec3& force, const glm::vec3& point)
{
	addForce(force);
	glm::vec3 r = point - _transform.getPosition();
	_torque += glm::cross(r, force);
}

void RigidBody::CalculateInertia()
{
	setInertia(GetCollider()->calculateLocalInertiaTensor(getMass()));
}

void RigidBody::clearForces()
{
	PhysicsObject::clearForces();
	_torque = glm::vec3(0.0f);
}
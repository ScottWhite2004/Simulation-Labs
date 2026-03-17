#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Collider.h"
#include "SphereCollider.h"
class PhysicsObject
{
public:
	enum class IntegrationMethod
	{
		Euler,
		SemiImplicitEuler
	};

private:
	IntegrationMethod _selectedIntegrationMethod{ IntegrationMethod::SemiImplicitEuler };
	
	glm::vec3 _position;

	glm::quat _orientation;

	glm::vec3 _velocity;

	glm::vec3 _angularVelocity;

	glm::vec3 _gravity{ 0.0f, 0.0f, 0.0f };

	SphereCollider _sphereCollider;

	glm::vec3 _accumulatedForces{ 0.0f, 0.0f, 0.0f };

	float _mass;

	float _inverseMass;

	

public:
	PhysicsObject(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& velocity, const SphereCollider& sphere, float mass = 1.0f) : _position(position), _orientation(orientation), _velocity(velocity), _sphereCollider(sphere), _mass(mass) { _inverseMass = (mass > 0.0f) ? 1.0f / _mass : 0.0f; }
	PhysicsObject();
	virtual ~PhysicsObject();
	glm::vec3 getPos() const { return _position; }
	glm::quat getOrientation() const { return _orientation; }
	glm::vec3 getVel() const { return _velocity; }
	glm::vec3 getAngularVel() const { return _angularVelocity; }
	float getMass() const { return _mass; }
	float getInverseMass() const { return _inverseMass; }
	const SphereCollider& GetSphere() const { return _sphereCollider; }
	SphereCollider& GetSphere() { return _sphereCollider; }
	IntegrationMethod getIntegrationMethod() const { return _selectedIntegrationMethod; }
	void SetPosition(const glm::vec3& newPosition) { _position = newPosition; }
	void SetOrientation(const glm::quat& newOrientation) { _orientation = newOrientation; }
	void SetVelocity(const glm::vec3& newVelocity) { _velocity = newVelocity; }
	void SetAngularVelocity(const glm::vec3& angularVelocity) { _angularVelocity = angularVelocity; }
	void SetMass(float mass) { _mass = mass; _inverseMass = (mass > 0.0f) ? 1.0f / mass : 0.0f; }
	void addForce(const glm::vec3& force) { _accumulatedForces += force; }
	void addAngularDisplacement(const glm::quat& displacement) { _orientation *= displacement; }
	void addAngularDisplacement(const glm::vec3& axis, const float& radians);
	void clearForces() { _accumulatedForces = glm::vec3(0.0f); }
	void IntegrateEuler(float seconds);
	void IntegrateEuler(float seconds, PhysicsObject& otherBody);
	void IntegrateEuler(float seconds, const Plane& staticPlane);
	void IntegrateSemiImplicitEuler(float seconds);
	void IntegrateSemiImplicitEuler(float seconds, PhysicsObject& otherBody);
	void IntegrateSemiImplicitEuler(float seconds, const Plane& staticPlane);
	void ResolveSphereSphereCollision(PhysicsObject& other);
};


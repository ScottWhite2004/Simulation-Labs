#pragma once
#include <glm/glm.hpp>
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

	glm::vec3 _rotation;

	glm::vec3 _velocity;

	glm::vec3 _gravity{ 0.0f, 0.0f, 0.0f };

	SphereCollider _sphereCollider;

	glm::vec3 _accumulatedForces{ 0.0f, 0.0f, 0.0f };

	float _mass;

	float _inverseMass;

	

public:
	PhysicsObject(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& velocity, const SphereCollider& sphere, float mass = 1.0f) : _position(position), _rotation(rotation), _velocity(velocity), _sphereCollider(sphere), _mass(mass) { _inverseMass = (mass > 0.0f) ? 1.0f / _mass : 0.0f; }
	PhysicsObject();
	virtual ~PhysicsObject();
	glm::vec3 getPos() const { return _position; }
	glm::vec3 getRot() const { return _rotation; }
	glm::vec3 getVel() const { return _velocity; }
	float getMass() const { return _mass; }
	float getInverseMass() const { return _inverseMass; }
	const SphereCollider& GetSphere() const { return _sphereCollider; }
	SphereCollider& GetSphere() { return _sphereCollider; }
	IntegrationMethod getIntegrationMethod() const { return _selectedIntegrationMethod; }
	void SetPosition(const glm::vec3& newPosition) { _position = newPosition; }
	void SetRotation(const glm::vec3& newRotation) { _rotation = newRotation; }
	void SetVelocity(const glm::vec3& newVelocity) { _velocity = newVelocity; }
	void SetMass(float mass) { _mass = mass; _inverseMass = (mass > 0.0f) ? 1.0f / mass : 0.0f; }
	void addForce(const glm::vec3& force) { _accumulatedForces += force; }
	void clearForces() { _accumulatedForces = glm::vec3(0.0f); }
	void IntegrateEuler(float seconds);
	void IntegrateEuler(float seconds, PhysicsObject& otherBody);
	void IntegrateEuler(float seconds, const Plane& staticPlane);
	void IntegrateSemiImplicitEuler(float seconds);
	void IntegrateSemiImplicitEuler(float seconds, PhysicsObject& otherBody);
	void IntegrateSemiImplicitEuler(float seconds, const Plane& staticPlane);
	void ResolveSphereSphereCollision(PhysicsObject& other);
};


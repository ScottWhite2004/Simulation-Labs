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
	
	glm::vec3 _position{ 0.0f, 0.0f, 0.0f };

	glm::quat _orientation{ 1.0f, 0.0f, 0.0f, 0.0f };

	glm::vec3 _velocity{ 0.0f, 0.0f, 0.0f };

	glm::vec3 _angularVelocity{ 0.0f, 0.0f, 0.0f };

	glm::vec3 _accumulatingTorque{ 0.0f, 0.0f, 0.0f };

	glm::vec3 _angularAcceleration{ 0.0f, 0.0f, 0.0f };

	glm::vec3 _centreOfMass{ 0.0f, 0.0f, 0.0f };

	glm::vec3 _gravity{ 0.0f, 0.0f, 0.0f };

	SphereCollider _sphereCollider{ glm::vec3(0.0f, 0.0f, 0.0f), 0.0f };

	glm::vec3 _accumulatedForces{ 0.0f, 0.0f, 0.0f };

	float _mass{ 1.0f };

	float _inverseMass{ 1.0f };

	float _inertia{ 1.0f };

	float _inverseInertia{ 1.0f };

	void calculateSphereInertia(const float& radius);

public:
	PhysicsObject(const glm::vec3& position, const glm::quat& orientation, const glm::vec3& velocity, const SphereCollider& sphere, float mass = 1.0f) : _position(position), _orientation(orientation), _velocity(velocity), _angularVelocity(0.0f), _accumulatingTorque(0.0f), _angularAcceleration(0.0f), _sphereCollider(sphere), _accumulatedForces(0.0f), _mass(mass), _inverseMass((mass > 0.0f) ? 1.0f / mass : 0.0f) { calculateSphereInertia(_sphereCollider.GetRadius()); }
	PhysicsObject();
	virtual ~PhysicsObject();
	glm::vec3 getPos() const { return _position; }
	glm::quat getOrientation() const { return _orientation; }
	glm::vec3 getVel() const { return _velocity; }
	glm::vec3 getAngularVel() const { return _angularVelocity; }
	glm::vec3 getTorque() const { return _accumulatingTorque; }
	glm::vec3 getAngularAcc() const { return _angularAcceleration; }
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
	void addTorque(const glm::vec3& torque) { _accumulatingTorque += torque; }
	void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
	void clearForces() { _accumulatedForces = glm::vec3(0.0f); _accumulatingTorque = glm::vec3(0.0f); }
	void IntegrateEuler(float seconds);
	void IntegrateEuler(float seconds, PhysicsObject& otherBody);
	void IntegrateEuler(float seconds, const Plane& staticPlane);
	void IntegrateSemiImplicitEuler(float seconds);
	void IntegrateSemiImplicitEuler(float seconds, PhysicsObject& otherBody);
	void IntegrateSemiImplicitEuler(float seconds, const Plane& staticPlane);
	void ResolveSphereSphereCollision(PhysicsObject& other);
};


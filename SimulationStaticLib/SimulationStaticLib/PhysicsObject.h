#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Collider.h"
#include "SphereCollider.h"
#include "Transform.h"
class PhysicsObject
{

private:

	Transform _transform;

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

	bool _isStatic{ false };

	void calculateSphereInertia(const float& radius);

public:
	PhysicsObject(const Transform& transform, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& velocity, const SphereCollider& sphere, float mass = 1.0f) : /*_transform(transform), */_position(position), _orientation(orientation), _velocity(velocity), _angularVelocity(0.0f), _accumulatingTorque(0.0f), _angularAcceleration(0.0f), _sphereCollider(sphere), _accumulatedForces(0.0f), _mass(mass), _inverseMass((mass > 0.0f) ? 1.0f / mass : 0.0f) { calculateSphereInertia(_sphereCollider.GetRadius()); }
	PhysicsObject();
	virtual ~PhysicsObject();
	Transform getTransform() const { return _transform; }
	glm::vec3 getVel() const { return _velocity; }
	glm::vec3 getAngularVel() const { return _angularVelocity; }
	glm::vec3 getTorque() const { return _accumulatingTorque; }
	glm::vec3 getAngularAcc() const { return _angularAcceleration; }
	float getMass() const { return _mass; }
	float getInverseMass() const { return _inverseMass; }
	const SphereCollider& GetSphere() const { return _sphereCollider; }
	SphereCollider& GetSphere() { return _sphereCollider; }
	void setTransform(const Transform& transform) { _transform = transform; }
	void SetVelocity(const glm::vec3& newVelocity) { _velocity = newVelocity; }
	void SetAngularVelocity(const glm::vec3& angularVelocity) { _angularVelocity = angularVelocity; }
	void SetMass(float mass) { _mass = mass; _inverseMass = (mass > 0.0f) ? 1.0f / mass : 0.0f; }
	void SetStatic(bool isStatic) { _isStatic = isStatic; }
	void addForce(const glm::vec3& force) { _accumulatedForces += force; }
	void addAngularDisplacement(const glm::quat& displacement) { _orientation *= displacement; }
	void addAngularDisplacement(const glm::vec3& axis, const float& radians);
	void addTorque(const glm::vec3& torque) { _accumulatingTorque += torque; }
	void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
	void clearForces() { _accumulatedForces = glm::vec3(0.0f); _accumulatingTorque = glm::vec3(0.0f); }
	void IntegrateEuler(float seconds);
	void IntegrateSemiImplicitEuler(float seconds);
	void ResolveSphereSphereCollision(PhysicsObject& other);
};


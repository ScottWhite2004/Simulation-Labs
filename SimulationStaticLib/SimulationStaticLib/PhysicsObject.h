#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Collider.h"
#include "SphereCollider.h"
#include "Transform.h"
class PhysicsObject
{

private:

	glm::vec3 _accumulatedForces{ 0.0f, 0.0f, 0.0f };

	float _mass{ 1.0f };

	float _inverseMass{ 1.0f };

	bool _isStatic{ false };

	Collider* _collider{ nullptr };

public:
	PhysicsObject();
	virtual ~PhysicsObject() = default;
	float getMass() const { return _mass; }
	float getInverseMass() const { return _inverseMass; }
	glm::vec3 getAccumulatedForces() const { return _accumulatedForces; }
	bool isStatic() const { return _isStatic; }
	void SetMass(float mass) { _mass = mass; _inverseMass = (mass > 0.0f) ? 1.0f / mass : 0.0f; }
	void SetStatic(bool isStatic) { _isStatic = isStatic; }
	void addForce(const glm::vec3& force) { _accumulatedForces += force; }
	void clearForces() { _accumulatedForces = glm::vec3(0.0f); }
	void SetCollider(Collider* collider) { _collider = collider; }
	Collider* GetCollider() const { return _collider; }
	virtual void IntegrateEuler(float seconds);
	virtual void IntegrateSemiImplicitEuler(float seconds);
};


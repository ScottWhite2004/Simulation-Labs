#pragma once
#include "CollisionEvent.h"
#include "RigidBody.h"
#include "Spring.h"
#include "glm/glm.hpp"
#include <vector>

enum class IntegrationMethod
{
	Euler,
	SemiImplicitEuler
};


class PhysicsWorld
{
	glm::vec3 _gravity;
	std::vector<PhysicsObject*> _objects;
	std::vector<Spring*> _springs;
	IntegrationMethod _selectedIntegrationMethod{ IntegrationMethod::SemiImplicitEuler };
	
	void applyGlobalForces(float deltaTime);
	void integrate(float deltaTime);
	void handleCollisions();

public:
	PhysicsWorld(const glm::vec3& gravity) : _gravity(gravity) {}
	void addObject(const PhysicsObject& object);
	void addSpring(const Spring& spring);
	void step(float deltaTime);
	void setIntegrationMethod(IntegrationMethod method);
	void setGravity(const glm::vec3& gravity);
};


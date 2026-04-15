#pragma once
#include "CollisionEvent.h"
#include "RigidBody.h"
#include "Spring.h"
#include "glm/glm.hpp"
#include <vector>


class PhysicsWorld
{
	glm::vec3 gravity;
	std::vector<RigidBody*> _rigidBodies;
	std::vector<Spring*> _springs;
	
	void applyGlobalForces(float deltaTime);
	void integrate(float deltaTime);
	void handleCollisions();

public:
	PhysicsWorld(const glm::vec3& gravity) : gravity(gravity) {}
	void addRigidBody(RigidBody* body);
	void addSpring(Spring* spring);
	void step(float deltaTime);
};


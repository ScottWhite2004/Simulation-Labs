#include "pch.h"
#include "PhysicsWorld.h"

void PhysicsWorld::addRigidBody(const RigidBody& body)
{
	_rigidBodies.push_back(new RigidBody(body));
}

void PhysicsWorld::addSpring(const Spring& spring)
{
	_springs.push_back(new Spring(spring));
}

void PhysicsWorld::step(float deltaTime)
{
	applyGlobalForces(deltaTime);
	integrate(deltaTime);
	handleCollisions();
}

void PhysicsWorld::applyGlobalForces(float deltaTime)
{
	for (auto& body : _rigidBodies)
	{
		//Apply gravity and other global forces to body
	}
}

void PhysicsWorld::integrate(float deltaTime)
{
	for (auto& body : _rigidBodies)
	{
		switch (_selectedIntegrationMethod)
		{
		case IntegrationMethod::Euler:
			//Integrate rigid body using Euler method
			break;
		case IntegrationMethod::SemiImplicitEuler:
			//Integrate rigid body using Semi-Implicit Euler method
			break;
		default:
			break;
		}
	}
}

void PhysicsWorld::setIntegrationMethod(IntegrationMethod method)
{
	_selectedIntegrationMethod = method;
}

void PhysicsWorld::handleCollisions()
{
	for (auto i = 0; i < _rigidBodies.size(); i++)
	{
		for (auto j = 0; j < _rigidBodies.size(); j++)
		{
			//Check for collisions between _rigidBodies[i] and _rigidBodies[j]
			//If collision detected, create CollisionEvent and resolve collision
		}
	}
}

void PhysicsWorld::setGravity(const glm::vec3& gravity)
{
	_gravity = gravity;
}

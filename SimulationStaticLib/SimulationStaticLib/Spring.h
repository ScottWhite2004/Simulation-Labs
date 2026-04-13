#pragma once
#include "PhysicsObject.h"
class Spring
{
	PhysicsObject* _objectA;
	PhysicsObject* _objectB;
	float _restLength;
	float _stiffness;
	float _damping;
	glm::vec3 _force;

public:
	Spring(PhysicsObject* objectA, PhysicsObject* objectB, float restLength, float stiffness, float damping) : _objectA(objectA), _objectB(objectB), _restLength(restLength), _stiffness(stiffness), _damping(damping) {}
	void update();
	void calculateForce();
};


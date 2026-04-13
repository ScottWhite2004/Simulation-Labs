#include "pch.h"
#include "Spring.h"

void Spring::update()
{
	calculateForce();
	_objectA->addForce(-_force);
	_objectB->addForce(_force);
}

void Spring::calculateForce()
{
	glm::vec3 delta = _objectB->getPos() - _objectA->getPos();
	float length = glm::length(delta);

	if (length <= 1e-6f)
	{
		_force = glm::vec3(0.0f);
		return;
	}

	glm::vec3 dir = delta / length;
	float x = length - _restLength; // extension (+) / compression (-)

	glm::vec3 relVel = _objectB->getVel() - _objectA->getVel();
	float vAlongSpring = glm::dot(relVel, dir);

	float springMag = -_stiffness * x;
	float dampingMag = -_damping * vAlongSpring;

	_force = (springMag + dampingMag) * dir;
}
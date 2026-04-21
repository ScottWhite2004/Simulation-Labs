#pragma once
#include "SpringParticle.h"
class Spring
{
    SpringParticle* _objectA;
	SpringParticle* _objectB;
	float _restLength;
	float _stiffness;
	float _damping;
	glm::vec3 _force{ 0.0f, 0.0f, 0.0f };

public:
 Spring(SpringParticle* objectA, SpringParticle* objectB, float restLength, float stiffness, float damping) : _objectA(objectA), _objectB(objectB), _restLength(restLength), _stiffness(stiffness), _damping(damping) {}
	void update();
	void calculateForce();
};


#include "pch.h"
#include "SpringParticle.h"

void SpringParticle::IntegrateEuler(float seconds)
{
	if (isStatic())
	{
		clearForces();
		return;
	}

	const glm::vec3 acceleration = getAccumulatedForces() * getInverseMass();
	_position += _velocity * seconds;
	_velocity += acceleration * seconds;

	clearForces();
}

void SpringParticle::IntegrateSemiImplicitEuler(float seconds)
{
	if (isStatic())
	{
		clearForces();
		return;
	}

	const glm::vec3 acceleration = getAccumulatedForces() * getInverseMass();
	_velocity += acceleration * seconds;
	_position += _velocity * seconds;

	clearForces();
}

#include "pch.h"
#include "CapsuleCollider.h"

glm::vec3 CapsuleCollider::calculateLocalInertiaTensor(float mass) const
{
	const float r = _radius;
	const float h = _height - 2.0f * r;

	const float denom = h + (4.0f / 3.0f) * r;
	const float cylinderMass = mass * (h / denom);
	const float hemisphereMass = mass * (((2.0f / 3.0f) * r) / denom); // each hemisphere

	const float inertiaY =
		0.5f * cylinderMass * r * r +
		2.0f * ((2.0f / 5.0f) * hemisphereMass * r * r);

	const float d = (0.5f * h) + (3.0f / 8.0f) * r;
	const float inertiaX =
		(1.0f / 12.0f) * cylinderMass * (3.0f * r * r + h * h) +
		2.0f * ((83.0f / 320.0f) * hemisphereMass * r * r + hemisphereMass * d * d);

	const float inertiaZ = inertiaX;

	return glm::vec3(inertiaX, inertiaY, inertiaZ);
}

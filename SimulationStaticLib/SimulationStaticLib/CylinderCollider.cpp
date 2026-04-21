#include "pch.h"
#include "CylinderCollider.h"

glm::vec3 CylinderCollider::calculateLocalInertiaTensor(float mass) const
{
	float inertiaX = (1.0f / 12.0f) * mass * (3 * _radius * _radius  + _height * _height);
	float inertiaY = (0.5f) * mass * _radius * _radius;
	float inertiaZ = inertiaX;
	return glm::vec3(inertiaX, inertiaY, inertiaZ);
}
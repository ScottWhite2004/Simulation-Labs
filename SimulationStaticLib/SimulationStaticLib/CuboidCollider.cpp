#include "pch.h"
#include "CuboidCollider.h"

glm::vec3 CuboidCollider::calculateLocalInertiaTensor(float mass) const
{ 
	float inertiaX = (1.0f / 12.0f) * mass * (_height * _height + _depth * _depth);
	float inertiaY = (1.0f / 12.0f) * mass * (_width * _width + _depth * _depth);
	float inertiaZ = (1.0f / 12.0f) * mass * (_width * _width + _height * _height);

	return glm::vec3(inertiaX, inertiaY, inertiaZ);
}
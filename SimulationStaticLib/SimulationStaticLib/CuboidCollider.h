#pragma once
#include "Collider.h"
class CuboidCollider : public Collider
{
	float _width;
	float _height;
	float _depth;
public:

	glm::vec3 calculateLocalInertiaTensor(float mass) const override;
};


#pragma once
#include "Collider.h"
class CylinderCollider : public Collider
{
	float _radius;
	float _height;

public:

	glm::vec3 calculateLocalInertiaTensor(float mass) const override;

};


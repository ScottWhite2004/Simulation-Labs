#pragma once
#include "Collider.h"

class Plane : public Collider
{

	glm::vec3 _Normal;

public:
	virtual bool IsInside(const glm::vec3& point) const override;

	virtual bool Intersects(const Line& line) const override;

};


#pragma once
#include "Collider.h"
#include "Line.h"
#include "glm/glm.hpp"

class Plane : public Collider
{

	glm::vec3 _Normal;

public:
	virtual bool IsInside(const glm::vec3& point) const override;

	virtual bool Intersects(const Line& line) const override;

	float ShortestDistanceToPoint(const glm::vec3& point) const;

	Plane(const glm::vec3& position, const glm::vec3& normal)
		: _Normal(normal)
	{
		_Position = position;
	}

};


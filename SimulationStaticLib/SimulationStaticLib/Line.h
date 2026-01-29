#pragma once
#include <glm/glm.hpp>
class Line
{
	glm::vec3 _Direction;
	float _Length;
	glm::vec3 _startingPoint;

public:

	Line(const glm::vec3& startingPoint, const glm::vec3& direction, float length)
		: _startingPoint(startingPoint), _Direction(direction), _Length(length)
	{
	}

	float ShortestDistanceToPoint(const glm::vec3& point) const;
};


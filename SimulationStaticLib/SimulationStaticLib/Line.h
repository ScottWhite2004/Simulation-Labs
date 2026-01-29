#pragma once
#include <glm/glm.hpp>
class Line
{
	glm::vec3 _Direction;
	glm::vec3 _pointOnLine;

public:

	Line(const glm::vec3& pointOnLine, const glm::vec3& direction)
		: _pointOnLine(pointOnLine), _Direction(direction)
	{
	}

	float ShortestDistanceToPoint(const glm::vec3& point) const;
};


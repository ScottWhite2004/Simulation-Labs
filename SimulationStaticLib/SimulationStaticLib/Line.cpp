#include "pch.h"
#include "Line.h"

float Line::ShortestDistanceToPoint(const glm::vec3& point) const
{
	glm::vec3 lineEndPoint = _startingPoint + _Direction * _Length;
	glm::vec3 lineVec = lineEndPoint - _startingPoint;
	glm::vec3 pointVec = point - _startingPoint;
	float lineLengthSquared = glm::dot(lineVec, lineVec);
	if (lineLengthSquared == 0.0f)
	{
		return glm::length(pointVec);
	}
	float t = glm::dot(pointVec, lineVec) / lineLengthSquared;
	t = glm::clamp(t, 0.0f, 1.0f);
	glm::vec3 projection = _startingPoint + t * lineVec;
	return glm::length(point - projection);
}

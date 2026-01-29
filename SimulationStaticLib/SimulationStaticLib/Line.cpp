#include "pch.h"
#include "Line.h"

float Line::ShortestDistanceToPoint(const glm::vec3& point) const
{
	glm::vec3 normalizedDirection = glm::normalize(_Direction);
	glm::vec3 pointToLineStart = point - _pointOnLine;
	float t = glm::dot(pointToLineStart, normalizedDirection);
	glm::vec3 closestPointOnLine = _pointOnLine + t * normalizedDirection;
	float shortestDistance = glm::length(point - closestPointOnLine);
	shortestDistance = roundf(shortestDistance * 100.0f) / 100.0f;
	return shortestDistance;
}

#include "pch.h"
#include "Sphere.h"

bool Sphere::IsInside(const glm::vec3& point) const
{
	glm::vec3 diff = point - _Position;
	float distanceSquared = glm::dot(diff, diff);
	return distanceSquared < (_Radius * _Radius);
}

bool Sphere::Intersects(const Line& line) const
{
	float distance = line.ShortestDistanceToPoint(_Position);
	return distance <= _Radius;
}

bool Sphere::CollidesWith(const Sphere& other) const
{
	glm::vec3 diff = other._Position - _Position;
	float distanceSquared = glm::dot(diff, diff);
	float radiusSum = _Radius + other._Radius;
	return distanceSquared <= (radiusSum * radiusSum);
}

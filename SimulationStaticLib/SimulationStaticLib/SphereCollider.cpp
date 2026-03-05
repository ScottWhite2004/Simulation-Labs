#include "pch.h"
#include "SphereCollider.h"


bool SphereCollider::IsInside(const glm::vec3& point) const
{
	glm::vec3 diff = point - _Position;
	float distanceSquared = glm::dot(diff, diff);
	return distanceSquared < (_Radius * _Radius);
}

bool SphereCollider::Intersects(const Line& line) const
{
	float distance = line.ShortestDistanceToPoint(_Position);
	return distance <= _Radius;
}

bool SphereCollider::CollidesWith(const SphereCollider& other) const
{
	glm::vec3 diff = other._Position - _Position;
	float distanceSquared = glm::dot(diff, diff);
	float radiusSum = _Radius + other._Radius;
	return distanceSquared <= (radiusSum * radiusSum);
}

bool SphereCollider::CollidesWith(const Plane& other) const
{
	float distance = other.ShortestDistanceToPoint(_Position);
	return fabs(distance) <= _Radius;
}

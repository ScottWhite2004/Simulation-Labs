#include "pch.h"
#include "PlaneCollider.h"


bool PlaneCollider::IsInside(const glm::vec3& point) const
{
    return false;
}

bool PlaneCollider::Intersects(const Line& line) const
{
    return false;
}

float PlaneCollider::ShortestDistanceToPoint(const glm::vec3& point) const
{
	glm::vec3 normalizedNormal = glm::normalize(_Normal);
	float distance = glm::dot(normalizedNormal, point - _Position);
	distance = roundf(distance * 100.0f) / 100.0f;
	return  fabs(distance);
}

glm::vec3 PlaneCollider::calculateLocalInertiaTensor(float mass) const
{
	return glm::vec3(0.0f);
}


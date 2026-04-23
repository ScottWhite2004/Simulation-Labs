#include "pch.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include <cmath>

bool PlaneCollider::IsInside(const glm::vec3& point) const
{
	const float d = glm::dot(glm::normalize(_Normal), point - _Position);
	return d <= 0.0f;
}

bool PlaneCollider::Intersects(const Line&) const
{
	return false;
}

float PlaneCollider::ShortestDistanceToPoint(const glm::vec3& point) const
{
	const glm::vec3 n = glm::normalize(_Normal);
	return std::fabs(glm::dot(n, point - _Position));
}

glm::vec3 PlaneCollider::calculateLocalInertiaTensor(float) const
{
	return glm::vec3(0.0f);
}

bool PlaneCollider::Collide(const Collider& other, CollisionEvent& outEvent) const
{
	return other.CollideWithPlane(*this, outEvent);
}

bool PlaneCollider::CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(_Normal);
	const float signedDistance = glm::dot(n, sphere.GetPosition() - _Position);
	const float absDistance = std::fabs(signedDistance);

	if (absDistance > sphere.GetRadius())
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = sphere.GetRadius() - absDistance;
	outEvent.collisionPoint = sphere.GetPosition() - normal * sphere.GetRadius();
	return true;
}

bool PlaneCollider::CollideWithPlane(const PlaneCollider&, CollisionEvent&) const
{
	return false;
}

bool PlaneCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(_Normal);
	const glm::vec3 h = cuboid.GetHalfExtents();
	const float projectedRadius = h.x * std::fabs(n.x) + h.y * std::fabs(n.y) + h.z * std::fabs(n.z);
	const float signedDistance = glm::dot(n, cuboid.GetPosition() - _Position);
	const float absDistance = std::fabs(signedDistance);

	if (absDistance > projectedRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = projectedRadius - absDistance;
	outEvent.collisionPoint = cuboid.GetPosition() - normal * absDistance;
	return true;
}

bool PlaneCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(_Normal);
	const float projectedRadius =
		cylinder.GetRadius() * std::sqrt((n.x * n.x) + (n.z * n.z)) +
		cylinder.GetHalfHeight() * std::fabs(n.y);

	const float signedDistance = glm::dot(n, cylinder.GetPosition() - _Position);
	const float absDistance = std::fabs(signedDistance);

	if (absDistance > projectedRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = projectedRadius - absDistance;
	outEvent.collisionPoint = cylinder.GetPosition() - normal * absDistance;
	return true;
}

bool PlaneCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(_Normal);
	const float projectedRadius = capsule.GetSegmentHalfLength() * std::fabs(n.y) + capsule.GetRadius();
	const float signedDistance = glm::dot(n, capsule.GetPosition() - _Position);
	const float absDistance = std::fabs(signedDistance);

	if (absDistance > projectedRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = projectedRadius - absDistance;
	outEvent.collisionPoint = capsule.GetPosition() - normal * absDistance;
	return true;
}
#include "pch.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include <algorithm>
#include <cmath>

namespace
{
	static float Clamp(float v, float lo, float hi)
	{
		return (v < lo) ? lo : ((v > hi) ? hi : v);
	}
}

bool SphereCollider::IsInside(const glm::vec3& point) const
{
	const glm::vec3 d = point - _transform.getPosition();
	return glm::dot(d, d) <= (_Radius * _Radius);
}

bool SphereCollider::Intersects(const Line& line) const
{
	return line.ShortestDistanceToPoint(_transform.getPosition()) <= _Radius;
}

glm::vec3 SphereCollider::calculateLocalInertiaTensor(float mass) const
{
	const float i = (2.0f / 5.0f) * mass * _Radius * _Radius;
	return glm::vec3(i, i, i);
}

bool SphereCollider::Collide(const Collider& other, CollisionEvent& outEvent) const
{
	return other.CollideWithSphere(*this, outEvent);
}

bool SphereCollider::CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const
{
	const float radiusA = _Radius * GetMaxAbsScale(_transform);
	const float radiusB = sphere.GetRadius() * GetMaxAbsScale(sphere.GetTransform());

	const glm::vec3 delta = sphere.GetTransform().getPosition() - _transform.getPosition();
	const float distSq = glm::dot(delta, delta);
	const float r = radiusA + radiusB;
	if (distSq > r * r)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 n = (dist > 1e-5f) ? (delta / dist) : glm::vec3(1.0f, 0.0f, 0.0f);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = r - dist;
	outEvent.collisionPoint = _transform.getPosition() + n * (radiusA - 0.5f * outEvent.penetrationDepth);
	return true;
}

bool SphereCollider::CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const
{
	return plane.CollideWithSphere(*this, outEvent);
}

bool SphereCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	const Transform& cuboidTransform = cuboid.GetTransform();
	const glm::vec3 localCenter = ToLocalNoScale(cuboidTransform, _transform.getPosition());
	const glm::vec3 halfExtents = cuboid.GetHalfExtents() * GetAbsScale(cuboidTransform);
	const float sphereRadius = _Radius * GetMaxAbsScale(_transform);

	const glm::vec3 closest(
		Clamp(localCenter.x, -halfExtents.x, halfExtents.x),
		Clamp(localCenter.y, -halfExtents.y, halfExtents.y),
		Clamp(localCenter.z, -halfExtents.z, halfExtents.z));

	const glm::vec3 d = localCenter - closest;
	const float distSq = glm::dot(d, d);
	if (distSq > sphereRadius * sphereRadius)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 localNormal = (dist > 1e-5f) ? (d / dist) : glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 worldNormal = glm::normalize(ToWorldDirNoScale(cuboidTransform, localNormal));

	outEvent.isColliding = true;
	outEvent.collisionNormal = worldNormal;
	outEvent.penetrationDepth = sphereRadius - dist;
	outEvent.collisionPoint = ToWorldNoScale(cuboidTransform, closest);
	return true;
}

bool SphereCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const Transform& cylinderTransform = cylinder.GetTransform();
	const glm::vec3 scale = GetAbsScale(cylinderTransform);

	const float radius = cylinder.GetRadius() * std::max(scale.x, scale.z);
	const float halfHeight = (cylinder.GetHeight() * 0.5f) * scale.y;
	const float sphereRadius = _Radius * GetMaxAbsScale(_transform);

	const glm::vec3 localSphere = ToLocalNoScale(cylinderTransform, _transform.getPosition());

	const float closestY = Clamp(localSphere.y, -halfHeight, halfHeight);

	glm::vec2 v(localSphere.x, localSphere.z);
	float len = glm::length(v);
	if (len > radius && len > 1e-6f)
	{
		v *= (radius / len);
	}

	const glm::vec3 closest(v.x, closestY, v.y);
	const glm::vec3 d = localSphere - closest;
	const float distSq = glm::dot(d, d);
	if (distSq > sphereRadius * sphereRadius)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 localNormal = (dist > 1e-5f) ? (d / dist) : glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 worldNormal = glm::normalize(ToWorldDirNoScale(cylinderTransform, localNormal));

	outEvent.isColliding = true;
	outEvent.collisionNormal = worldNormal;
	outEvent.penetrationDepth = sphereRadius - dist;
	outEvent.collisionPoint = ToWorldNoScale(cylinderTransform, closest);
	return true;
}

bool SphereCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const Transform& capsuleTransform = capsule.GetTransform();
	const glm::vec3 scale = GetAbsScale(capsuleTransform);

	const float capsuleRadius = capsule.GetRadius() * std::max(scale.x, scale.z);
	const float capsuleHalfHeight = (capsule.GetHeight() * 0.5f) * scale.y;
	const float segmentHalf = std::max(0.0f, capsuleHalfHeight - capsuleRadius);
	const float sphereRadius = _Radius * GetMaxAbsScale(_transform);

	const glm::vec3 localSphere = ToLocalNoScale(capsuleTransform, _transform.getPosition());
	const float yOnSegment = Clamp(localSphere.y, -segmentHalf, segmentHalf);
	const glm::vec3 closest(0.0f, yOnSegment, 0.0f);

	const glm::vec3 d = localSphere - closest;
	const float distSq = glm::dot(d, d);
	const float r = sphereRadius + capsuleRadius;
	if (distSq > r * r)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 localNormal = (dist > 1e-5f) ? (d / dist) : glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 worldNormal = glm::normalize(ToWorldDirNoScale(capsuleTransform, localNormal));

	outEvent.isColliding = true;
	outEvent.collisionNormal = worldNormal;
	outEvent.penetrationDepth = r - dist;
	outEvent.collisionPoint = ToWorldNoScale(capsuleTransform, closest + localNormal * capsuleRadius);
	return true;
}
#include "pch.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include <cmath>

namespace
{
	static float Clamp(float v, float lo, float hi)
	{
		return (v < lo) ? lo : ((v > hi) ? hi : v);
	}
}

SphereCollider::SphereCollider() = default;

bool SphereCollider::IsInside(const glm::vec3& point) const
{
	const glm::vec3 d = point - _Position;
	return glm::dot(d, d) <= (_Radius * _Radius);
}

bool SphereCollider::Intersects(const Line& line) const
{
	return line.ShortestDistanceToPoint(_Position) <= _Radius;
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
	const glm::vec3 delta = sphere.GetPosition() - _Position;
	const float distSq = glm::dot(delta, delta);
	const float r = _Radius + sphere.GetRadius();
	if (distSq > r * r)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 n = (dist > 1e-5f) ? (delta / dist) : glm::vec3(1.0f, 0.0f, 0.0f);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = r - dist;
	outEvent.collisionPoint = _Position + n * (_Radius - 0.5f * outEvent.penetrationDepth);
	return true;
}

bool SphereCollider::CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const
{
	return plane.CollideWithSphere(*this, outEvent);
}

bool SphereCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	const glm::vec3 c = cuboid.GetPosition();
	const glm::vec3 h = cuboid.GetHalfExtents();

	const glm::vec3 closest(
		Clamp(_Position.x, c.x - h.x, c.x + h.x),
		Clamp(_Position.y, c.y - h.y, c.y + h.y),
		Clamp(_Position.z, c.z - h.z, c.z + h.z));

	const glm::vec3 d = _Position - closest;
	const float distSq = glm::dot(d, d);
	if (distSq > _Radius * _Radius)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 n = (dist > 1e-5f) ? (d / dist) : glm::vec3(0.0f, 1.0f, 0.0f);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = _Radius - dist;
	outEvent.collisionPoint = closest;
	return true;
}

bool SphereCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const glm::vec3 cc = cylinder.GetPosition();
	const float hh = cylinder.GetHalfHeight();

	const float closestY = Clamp(_Position.y, cc.y - hh, cc.y + hh);

	glm::vec2 v(_Position.x - cc.x, _Position.z - cc.z);
	float len = glm::length(v);
	if (len > cylinder.GetRadius() && len > 1e-6f)
	{
		v *= (cylinder.GetRadius() / len);
	}

	const glm::vec3 closest(cc.x + v.x, closestY, cc.z + v.y);
	const glm::vec3 d = _Position - closest;
	const float distSq = glm::dot(d, d);
	if (distSq > _Radius * _Radius)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 n = (dist > 1e-5f) ? (d / dist) : glm::vec3(0.0f, 1.0f, 0.0f);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = _Radius - dist;
	outEvent.collisionPoint = closest;
	return true;
}

bool SphereCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 c = capsule.GetPosition();
	const float hs = capsule.GetSegmentHalfLength();

	const float yOnSegment = Clamp(_Position.y, c.y - hs, c.y + hs);
	const glm::vec3 closest(c.x, yOnSegment, c.z);

	const glm::vec3 d = _Position - closest;
	const float distSq = glm::dot(d, d);
	const float r = _Radius + capsule.GetRadius();
	if (distSq > r * r)
	{
		return false;
	}

	const float dist = std::sqrt((distSq > 1e-8f) ? distSq : 1e-8f);
	const glm::vec3 n = (dist > 1e-5f) ? (d / dist) : glm::vec3(0.0f, 1.0f, 0.0f);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = r - dist;
	outEvent.collisionPoint = closest + n * capsule.GetRadius();
	return true;
}
#include "pch.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include <cmath>

namespace
{
	static float Clamp(float v, float lo, float hi)
	{
		return (v < lo) ? lo : ((v > hi) ? hi : v);
	}
}

glm::vec3 CapsuleCollider::calculateLocalInertiaTensor(float mass) const
{
	const float r = _radius;
	const float h = _height - 2.0f * r;

	const float denom = h + (4.0f / 3.0f) * r;
	const float cylinderMass = mass * (h / denom);
	const float hemisphereMass = mass * (((2.0f / 3.0f) * r) / denom);

	const float inertiaY =
		0.5f * cylinderMass * r * r +
		2.0f * ((2.0f / 5.0f) * hemisphereMass * r * r);

	const float d = (0.5f * h) + (3.0f / 8.0f) * r;
	const float inertiaX =
		(1.0f / 12.0f) * cylinderMass * (3.0f * r * r + h * h) +
		2.0f * ((83.0f / 320.0f) * hemisphereMass * r * r + hemisphereMass * d * d);

	return glm::vec3(inertiaX, inertiaY, inertiaX);
}

bool CapsuleCollider::IsInside(const glm::vec3& point) const
{
	const float hs = GetSegmentHalfLength();
	const float y = Clamp(point.y, _Position.y - hs, _Position.y + hs);
	const glm::vec3 closest(_Position.x, y, _Position.z);
	const glm::vec3 d = point - closest;
	return glm::dot(d, d) <= (_radius * _radius);
}

bool CapsuleCollider::Intersects(const Line& line) const
{
	const float bound = GetSegmentHalfLength() + _radius;
	return line.ShortestDistanceToPoint(_Position) <= bound;
}

bool CapsuleCollider::Collide(const Collider& other, CollisionEvent& outEvent) const
{
	return other.CollideWithCapsule(*this, outEvent);
}

bool CapsuleCollider::CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const
{
	return sphere.CollideWithCapsule(*this, outEvent);
}

bool CapsuleCollider::CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const
{
	return plane.CollideWithCapsule(*this, outEvent);
}

bool CapsuleCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	return cuboid.CollideWithCapsule(*this, outEvent);
}

bool CapsuleCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	return cylinder.CollideWithCapsule(*this, outEvent);
}

bool CapsuleCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 d = capsule.GetPosition() - _Position;
	const float distXZSq = (d.x * d.x) + (d.z * d.z);
	const float r = _radius + capsule.GetRadius();
	if (distXZSq > r * r)
	{
		return false;
	}

	const float fullHalfA = GetSegmentHalfLength() + _radius;
	const float fullHalfB = capsule.GetSegmentHalfLength() + capsule.GetRadius();
	const float yOverlap = (fullHalfA + fullHalfB) - std::fabs(d.y);
	if (yOverlap <= 0.0f)
	{
		return false;
	}

	const float distXZ = std::sqrt((distXZSq > 1e-8f) ? distXZSq : 1e-8f);
	const float radialPen = r - distXZ;

	outEvent.isColliding = true;
	if (radialPen < yOverlap)
	{
		outEvent.penetrationDepth = radialPen;
		outEvent.collisionNormal = (distXZ > 1e-5f) ? glm::vec3(d.x / distXZ, 0.0f, d.z / distXZ) : glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		outEvent.penetrationDepth = yOverlap;
		outEvent.collisionNormal = glm::vec3(0.0f, (d.y >= 0.0f) ? 1.0f : -1.0f, 0.0f);
	}
	outEvent.collisionPoint = 0.5f * (_Position + capsule.GetPosition());
	return true;
}
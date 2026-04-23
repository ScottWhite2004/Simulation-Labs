#include "pch.h"
#include "CylinderCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CuboidCollider.h"
#include "CapsuleCollider.h"
#include <cmath>

glm::vec3 CylinderCollider::calculateLocalInertiaTensor(float mass) const
{
	const float r2 = _radius * _radius;
	const float h2 = _height * _height;
	return glm::vec3(
		(mass / 12.0f) * (3.0f * r2 + h2),
		0.5f * mass * r2,
		(mass / 12.0f) * (3.0f * r2 + h2));
}

bool CylinderCollider::IsInside(const glm::vec3& point) const
{
	const glm::vec3 d = point - _Position;
	const float radialSq = (d.x * d.x) + (d.z * d.z);
	return radialSq <= (_radius * _radius) && std::fabs(d.y) <= GetHalfHeight();
}

bool CylinderCollider::Intersects(const Line& line) const
{
	const float bound = std::sqrt(_radius * _radius + GetHalfHeight() * GetHalfHeight());
	return line.ShortestDistanceToPoint(_Position) <= bound;
}

bool CylinderCollider::Collide(const Collider& other, CollisionEvent& outEvent) const
{
	return other.CollideWithCylinder(*this, outEvent);
}

bool CylinderCollider::CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const
{
	return sphere.CollideWithCylinder(*this, outEvent);
}

bool CylinderCollider::CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const
{
	return plane.CollideWithCylinder(*this, outEvent);
}

bool CylinderCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	return cuboid.CollideWithCylinder(*this, outEvent);
}

bool CylinderCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const glm::vec3 d = cylinder.GetPosition() - _Position;
	const float distXZSq = (d.x * d.x) + (d.z * d.z);
	const float r = _radius + cylinder.GetRadius();
	if (distXZSq > r * r)
	{
		return false;
	}

	const float yOverlap = (GetHalfHeight() + cylinder.GetHalfHeight()) - std::fabs(d.y);
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
	outEvent.collisionPoint = 0.5f * (_Position + cylinder.GetPosition());
	return true;
}

bool CylinderCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 d = capsule.GetPosition() - _Position;
	const float distXZSq = (d.x * d.x) + (d.z * d.z);
	const float r = _radius + capsule.GetRadius();
	if (distXZSq > r * r)
	{
		return false;
	}

	const float capHalf = capsule.GetSegmentHalfLength() + capsule.GetRadius();
	const float yOverlap = (GetHalfHeight() + capHalf) - std::fabs(d.y);
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
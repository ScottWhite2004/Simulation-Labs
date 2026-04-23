#include "pch.h"
#include "CuboidCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
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

glm::vec3 CuboidCollider::calculateLocalInertiaTensor(float mass) const
{
	const float x2 = _width * _width;
	const float y2 = _height * _height;
	const float z2 = _depth * _depth;
	return glm::vec3(
		(mass / 12.0f) * (y2 + z2),
		(mass / 12.0f) * (x2 + z2),
		(mass / 12.0f) * (x2 + y2));
}

bool CuboidCollider::IsInside(const glm::vec3& point) const
{
	const glm::vec3 h = GetHalfExtents();
	return std::fabs(point.x - _Position.x) <= h.x &&
		std::fabs(point.y - _Position.y) <= h.y &&
		std::fabs(point.z - _Position.z) <= h.z;
}

bool CuboidCollider::Intersects(const Line& line) const
{
	const glm::vec3 h = GetHalfExtents();
	const float r = glm::length(h);
	return line.ShortestDistanceToPoint(_Position) <= r;
}

bool CuboidCollider::Collide(const Collider& other, CollisionEvent& outEvent) const
{
	return other.CollideWithCuboid(*this, outEvent);
}

bool CuboidCollider::CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const
{
	return sphere.CollideWithCuboid(*this, outEvent);
}

bool CuboidCollider::CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const
{
	return plane.CollideWithCuboid(*this, outEvent);
}

bool CuboidCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	const glm::vec3 aH = GetHalfExtents();
	const glm::vec3 bH = cuboid.GetHalfExtents();
	const glm::vec3 d = cuboid.GetPosition() - _Position;

	const float ox = (aH.x + bH.x) - std::fabs(d.x);
	const float oy = (aH.y + bH.y) - std::fabs(d.y);
	const float oz = (aH.z + bH.z) - std::fabs(d.z);

	if (ox <= 0.0f || oy <= 0.0f || oz <= 0.0f)
	{
		return false;
	}

	outEvent.isColliding = true;
	outEvent.collisionPoint = 0.5f * (_Position + cuboid.GetPosition());

	if (ox <= oy && ox <= oz)
	{
		outEvent.penetrationDepth = ox;
		outEvent.collisionNormal = glm::vec3((d.x >= 0.0f) ? 1.0f : -1.0f, 0.0f, 0.0f);
	}
	else if (oy <= ox && oy <= oz)
	{
		outEvent.penetrationDepth = oy;
		outEvent.collisionNormal = glm::vec3(0.0f, (d.y >= 0.0f) ? 1.0f : -1.0f, 0.0f);
	}
	else
	{
		outEvent.penetrationDepth = oz;
		outEvent.collisionNormal = glm::vec3(0.0f, 0.0f, (d.z >= 0.0f) ? 1.0f : -1.0f);
	}

	return true;
}

bool CuboidCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const glm::vec3 h = GetHalfExtents();
	const glm::vec3 c = cylinder.GetPosition();

	const float dx = c.x - Clamp(c.x, _Position.x - h.x, _Position.x + h.x);
	const float dz = c.z - Clamp(c.z, _Position.z - h.z, _Position.z + h.z);
	const float distXZSq = dx * dx + dz * dz;
	if (distXZSq > cylinder.GetRadius() * cylinder.GetRadius())
	{
		return false;
	}

	const float yOverlap = (h.y + cylinder.GetHalfHeight()) - std::fabs(c.y - _Position.y);
	if (yOverlap <= 0.0f)
	{
		return false;
	}

	outEvent.isColliding = true;
	const float distXZ = std::sqrt((distXZSq > 1e-8f) ? distXZSq : 1e-8f);
	const float radialPen = cylinder.GetRadius() - distXZ;

	if (radialPen < yOverlap)
	{
		outEvent.penetrationDepth = radialPen;
		outEvent.collisionNormal = (distXZ > 1e-5f) ? glm::vec3(dx / distXZ, 0.0f, dz / distXZ) : glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		outEvent.penetrationDepth = yOverlap;
		outEvent.collisionNormal = glm::vec3(0.0f, (c.y >= _Position.y) ? 1.0f : -1.0f, 0.0f);
	}

	outEvent.collisionPoint = glm::vec3(
		Clamp(c.x, _Position.x - h.x, _Position.x + h.x),
		Clamp(c.y, _Position.y - h.y, _Position.y + h.y),
		Clamp(c.z, _Position.z - h.z, _Position.z + h.z));
	return true;
}

bool CuboidCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 h = GetHalfExtents();
	const glm::vec3 c = capsule.GetPosition();

	const float dx = c.x - Clamp(c.x, _Position.x - h.x, _Position.x + h.x);
	const float dz = c.z - Clamp(c.z, _Position.z - h.z, _Position.z + h.z);
	const float distXZSq = dx * dx + dz * dz;
	if (distXZSq > capsule.GetRadius() * capsule.GetRadius())
	{
		return false;
	}

	const float fullHalfHeight = capsule.GetSegmentHalfLength() + capsule.GetRadius();
	const float yOverlap = (h.y + fullHalfHeight) - std::fabs(c.y - _Position.y);
	if (yOverlap <= 0.0f)
	{
		return false;
	}

	outEvent.isColliding = true;
	const float distXZ = std::sqrt((distXZSq > 1e-8f) ? distXZSq : 1e-8f);
	const float radialPen = capsule.GetRadius() - distXZ;

	if (radialPen < yOverlap)
	{
		outEvent.penetrationDepth = radialPen;
		outEvent.collisionNormal = (distXZ > 1e-5f) ? glm::vec3(dx / distXZ, 0.0f, dz / distXZ) : glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		outEvent.penetrationDepth = yOverlap;
		outEvent.collisionNormal = glm::vec3(0.0f, (c.y >= _Position.y) ? 1.0f : -1.0f, 0.0f);
	}

	outEvent.collisionPoint = glm::vec3(
		Clamp(c.x, _Position.x - h.x, _Position.x + h.x),
		Clamp(c.y, _Position.y - h.y, _Position.y + h.y),
		Clamp(c.z, _Position.z - h.z, _Position.z + h.z));
	return true;
}
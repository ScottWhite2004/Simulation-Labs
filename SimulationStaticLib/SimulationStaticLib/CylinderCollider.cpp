#include "pch.h"
#include "CylinderCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CuboidCollider.h"
#include "CapsuleCollider.h"
#include <algorithm>
#include <cmath>

namespace
{
	static float Clamp01(float v)
	{
		return (v < 0.0f) ? 0.0f : ((v > 1.0f) ? 1.0f : v);
	}

	static float SegmentSegmentDistanceSquared(
		const glm::vec3& p1,
		const glm::vec3& q1,
		const glm::vec3& p2,
		const glm::vec3& q2,
		glm::vec3& c1,
		glm::vec3& c2)
	{
		const glm::vec3 d1 = q1 - p1;
		const glm::vec3 d2 = q2 - p2;
		const glm::vec3 r = p1 - p2;
		const float a = glm::dot(d1, d1);
		const float e = glm::dot(d2, d2);
		const float f = glm::dot(d2, r);
		const float epsilon = 1e-6f;

		float s = 0.0f;
		float t = 0.0f;

		if (a <= epsilon && e <= epsilon)
		{
			c1 = p1;
			c2 = p2;
			return glm::dot(c1 - c2, c1 - c2);
		}

		if (a <= epsilon)
		{
			s = 0.0f;
			t = Clamp01(f / e);
		}
		else
		{
			const float c = glm::dot(d1, r);
			if (e <= epsilon)
			{
				t = 0.0f;
				s = Clamp01(-c / a);
			}
			else
			{
				const float b = glm::dot(d1, d2);
				const float denom = a * e - b * b;

				if (denom != 0.0f)
				{
					s = Clamp01((b * f - c * e) / denom);
				}
				else
				{
					s = 0.0f;
				}

				t = (b * s + f) / e;

				if (t < 0.0f)
				{
					t = 0.0f;
					s = Clamp01(-c / a);
				}
				else if (t > 1.0f)
				{
					t = 1.0f;
					s = Clamp01((b - c) / a);
				}
			}
		}

		c1 = p1 + d1 * s;
		c2 = p2 + d2 * t;
		return glm::dot(c1 - c2, c1 - c2);
	}
}

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
	const glm::vec3 d = point - _transform.getPosition();
	const float radialSq = (d.x * d.x) + (d.z * d.z);
	return radialSq <= (_radius * _radius) && std::fabs(d.y) <= GetHalfHeight();
}

bool CylinderCollider::Intersects(const Line& line) const
{
	const float bound = std::sqrt(_radius * _radius + GetHalfHeight() * GetHalfHeight());
	return line.ShortestDistanceToPoint(_transform.getPosition()) <= bound;
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
	const glm::vec3 aScale = GetAbsScale(_transform);
	const glm::vec3 bScale = GetAbsScale(cylinder.GetTransform());

	const float radiusA = _radius * std::max(aScale.x, aScale.z);
	const float radiusB = cylinder.GetRadius() * std::max(bScale.x, bScale.z);

	const float halfHeightA = (_height * 0.5f) * aScale.y;
	const float halfHeightB = (cylinder.GetHeight() * 0.5f) * bScale.y;

	const glm::vec3 axisA = glm::normalize(ToWorldDirNoScale(_transform, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 axisB = glm::normalize(ToWorldDirNoScale(cylinder.GetTransform(), glm::vec3(0.0f, 1.0f, 0.0f)));

	const glm::vec3 centerA = _transform.getPosition();
	const glm::vec3 centerB = cylinder.GetTransform().getPosition();

	const glm::vec3 a0 = centerA - axisA * halfHeightA;
	const glm::vec3 a1 = centerA + axisA * halfHeightA;
	const glm::vec3 b0 = centerB - axisB * halfHeightB;
	const glm::vec3 b1 = centerB + axisB * halfHeightB;

	glm::vec3 c1;
	glm::vec3 c2;
	const float distSq = SegmentSegmentDistanceSquared(a0, a1, b0, b1, c1, c2);
	const float radiusSum = radiusA + radiusB;

	if (distSq > radiusSum * radiusSum)
	{
		return false;
	}

	const float dist = std::sqrt(std::max(distSq, 1e-8f));
	const glm::vec3 n = (dist > 1e-5f)
		? glm::normalize(c2 - c1)
		: glm::normalize(centerB - centerA);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = radiusSum - dist;
	outEvent.collisionPoint = 0.5f * (c1 + c2);
	return true;
}

bool CylinderCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 aScale = GetAbsScale(_transform);
	const glm::vec3 bScale = GetAbsScale(capsule.GetTransform());

	const float radiusA = _radius * std::max(aScale.x, aScale.z);
	const float radiusB = capsule.GetRadius() * std::max(bScale.x, bScale.z);

	const float halfHeightA = (_height * 0.5f) * aScale.y;
	const float capsuleHalfHeight = (capsule.GetHeight() * 0.5f) * bScale.y;
	const float segmentHalf = std::max(0.0f, capsuleHalfHeight - radiusB);

	const glm::vec3 axisA = glm::normalize(ToWorldDirNoScale(_transform, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 axisB = glm::normalize(ToWorldDirNoScale(capsule.GetTransform(), glm::vec3(0.0f, 1.0f, 0.0f)));

	const glm::vec3 centerA = _transform.getPosition();
	const glm::vec3 centerB = capsule.GetTransform().getPosition();

	const glm::vec3 a0 = centerA - axisA * halfHeightA;
	const glm::vec3 a1 = centerA + axisA * halfHeightA;
	const glm::vec3 b0 = centerB - axisB * segmentHalf;
	const glm::vec3 b1 = centerB + axisB * segmentHalf;

	glm::vec3 c1;
	glm::vec3 c2;
	const float distSq = SegmentSegmentDistanceSquared(a0, a1, b0, b1, c1, c2);
	const float radiusSum = radiusA + radiusB;

	if (distSq > radiusSum * radiusSum)
	{
		return false;
	}

	const float dist = std::sqrt(std::max(distSq, 1e-8f));
	const glm::vec3 n = (dist > 1e-5f)
		? glm::normalize(c2 - c1)
		: glm::normalize(centerB - centerA);

	outEvent.isColliding = true;
	outEvent.collisionNormal = n;
	outEvent.penetrationDepth = radiusSum - dist;
	outEvent.collisionPoint = 0.5f * (c1 + c2);
	return true;
}
#include "pch.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include <algorithm>
#include <cmath>

namespace
{
	static float Clamp(float v, float lo, float hi)
	{
		return (v < lo) ? lo : ((v > hi) ? hi : v);
	}

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
	const float y = Clamp(point.y, _transform.getPosition().y - hs, _transform.getPosition().y + hs);
	const glm::vec3 closest(_transform.getPosition().x, y, _transform.getPosition().z);
	const glm::vec3 d = point - closest;
	return glm::dot(d, d) <= (_radius * _radius);
}

bool CapsuleCollider::Intersects(const Line& line) const
{
	const float bound = GetSegmentHalfLength() + _radius;
	return line.ShortestDistanceToPoint(_transform.getPosition()) <= bound;
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
	const glm::vec3 aScale = GetAbsScale(_transform);
	const glm::vec3 bScale = GetAbsScale(capsule.GetTransform());

	const float radiusA = _radius * std::max(aScale.x, aScale.z);
	const float radiusB = capsule.GetRadius() * std::max(bScale.x, bScale.z);

	const float halfHeightA = (_height * 0.5f) * aScale.y;
	const float halfHeightB = (capsule.GetHeight() * 0.5f) * bScale.y;

	const float segmentHalfA = std::max(0.0f, halfHeightA - radiusA);
	const float segmentHalfB = std::max(0.0f, halfHeightB - radiusB);

	const glm::vec3 axisA = glm::normalize(ToWorldDirNoScale(_transform, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 axisB = glm::normalize(ToWorldDirNoScale(capsule.GetTransform(), glm::vec3(0.0f, 1.0f, 0.0f)));

	const glm::vec3 centerA = _transform.getPosition();
	const glm::vec3 centerB = capsule.GetTransform().getPosition();

	const glm::vec3 a0 = centerA - axisA * segmentHalfA;
	const glm::vec3 a1 = centerA + axisA * segmentHalfA;
	const glm::vec3 b0 = centerB - axisB * segmentHalfB;
	const glm::vec3 b1 = centerB + axisB * segmentHalfB;

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
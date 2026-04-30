#include "pch.h"
#include "CuboidCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include <algorithm>
#include <cfloat>
#include <cmath>

namespace
{
	static float Clamp(float v, float lo, float hi)
	{
		return (v < lo) ? lo : ((v > hi) ? hi : v);
	}

	static float DistanceSqPointAABB(const glm::vec3& point, const glm::vec3& halfExtents, glm::vec3& outClosest)
	{
		outClosest = glm::vec3(
			Clamp(point.x, -halfExtents.x, halfExtents.x),
			Clamp(point.y, -halfExtents.y, halfExtents.y),
			Clamp(point.z, -halfExtents.z, halfExtents.z));

		const glm::vec3 d = point - outClosest;
		return glm::dot(d, d);
	}

	static float ClosestPointSegmentAABB(
		const glm::vec3& p0,
		const glm::vec3& p1,
		const glm::vec3& halfExtents,
		glm::vec3& outSegPoint,
		glm::vec3& outBoxPoint)
	{
		const glm::vec3 d = p1 - p0;
		float t0 = 0.0f;
		float t1 = 1.0f;

		for (int i = 0; i < 20; ++i)
		{
			const float tA = (2.0f * t0 + t1) / 3.0f;
			const float tB = (t0 + 2.0f * t1) / 3.0f;

			glm::vec3 tempA;
			glm::vec3 tempB;

			const float distA = DistanceSqPointAABB(p0 + d * tA, halfExtents, tempA);
			const float distB = DistanceSqPointAABB(p0 + d * tB, halfExtents, tempB);

			if (distA < distB)
			{
				t1 = tB;
			}
			else
			{
				t0 = tA;
			}
		}

		const float t = 0.5f * (t0 + t1);
		outSegPoint = p0 + d * t;
		return DistanceSqPointAABB(outSegPoint, halfExtents, outBoxPoint);
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
	const glm::vec3 localPoint = ToLocalNoScale(_transform, point);
	const glm::vec3 halfExtents = GetHalfExtents() * GetAbsScale(_transform);

	return std::fabs(localPoint.x) <= halfExtents.x &&
		std::fabs(localPoint.y) <= halfExtents.y &&
		std::fabs(localPoint.z) <= halfExtents.z;
}

bool CuboidCollider::Intersects(const Line& line) const
{
	const glm::vec3 h = GetHalfExtents();
	const float r = glm::length(h);
	return line.ShortestDistanceToPoint(_transform.getPosition()) <= r;
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
	const Transform& aTransform = _transform;
	const Transform& bTransform = cuboid.GetTransform();

	const glm::vec3 aH = GetHalfExtents() * GetAbsScale(aTransform);
	const glm::vec3 bH = cuboid.GetHalfExtents() * GetAbsScale(bTransform);

	const glm::vec3 aAxis[3] =
	{
		glm::normalize(ToWorldDirNoScale(aTransform, glm::vec3(1, 0, 0))),
		glm::normalize(ToWorldDirNoScale(aTransform, glm::vec3(0, 1, 0))),
		glm::normalize(ToWorldDirNoScale(aTransform, glm::vec3(0, 0, 1)))
	};

	const glm::vec3 bAxis[3] =
	{
		glm::normalize(ToWorldDirNoScale(bTransform, glm::vec3(1, 0, 0))),
		glm::normalize(ToWorldDirNoScale(bTransform, glm::vec3(0, 1, 0))),
		glm::normalize(ToWorldDirNoScale(bTransform, glm::vec3(0, 0, 1)))
	};

	float R[3][3];
	float AbsR[3][3];

	const float epsilon = 1e-6f;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			R[i][j] = glm::dot(aAxis[i], bAxis[j]);
			AbsR[i][j] = std::fabs(R[i][j]) + epsilon;
		}
	}

	const glm::vec3 tWorld = bTransform.getPosition() - aTransform.getPosition();

	const glm::vec3 t(
		glm::dot(tWorld, aAxis[0]),
		glm::dot(tWorld, aAxis[1]),
		glm::dot(tWorld, aAxis[2]));

	// ============================================================
	// CONTAINER MODE (this cuboid is hollow, other cuboid must stay inside)
	// ============================================================
	if (IsContainer())
	{
		float minEscape = FLT_MAX;
		glm::vec3 bestNormal(1, 0, 0);

		for (int i = 0; i < 3; ++i)
		{
			// projection of B half extents onto A axis i
			const float rb =
				bH.x * AbsR[i][0] +
				bH.y * AbsR[i][1] +
				bH.z * AbsR[i][2];

			// amount beyond container wall
			const float outside = std::fabs(t[i]) + rb - aH[i];

			if (outside > 0.0f)
			{
				if (outside < minEscape)
				{
					minEscape = outside;

					glm::vec3 n = aAxis[i];

					// inward push
					if (t[i] > 0.0f)
						n = -n;

					bestNormal = n;
				}
			}
		}

		if (minEscape < FLT_MAX)
		{
			outEvent.isColliding = true;
			outEvent.collisionNormal = bestNormal;
			outEvent.penetrationDepth = minEscape;
			outEvent.collisionPoint = bTransform.getPosition();
			return true;
		}

		return false; // fully inside container
	}

	// ============================================================
	// NORMAL SOLID OBB vs OBB SAT
	// ============================================================

	float minOverlap = FLT_MAX;
	glm::vec3 minAxis = aAxis[0];

	auto UpdateMin = [&](const glm::vec3& axis, float overlap)
		{
			if (overlap < minOverlap)
			{
				minOverlap = overlap;
				minAxis = axis;
			}
		};

	// A face axes
	for (int i = 0; i < 3; ++i)
	{
		const float ra = aH[i];

		const float rb =
			bH.x * AbsR[i][0] +
			bH.y * AbsR[i][1] +
			bH.z * AbsR[i][2];

		const float dist = std::fabs(t[i]);
		const float overlap = ra + rb - dist;

		if (overlap < 0.0f)
			return false;

		UpdateMin(aAxis[i], overlap);
	}

	// B face axes
	for (int j = 0; j < 3; ++j)
	{
		const float ra =
			aH.x * AbsR[0][j] +
			aH.y * AbsR[1][j] +
			aH.z * AbsR[2][j];

		const float rb = bH[j];

		const float dist = std::fabs(glm::dot(tWorld, bAxis[j]));
		const float overlap = ra + rb - dist;

		if (overlap < 0.0f)
			return false;

		UpdateMin(bAxis[j], overlap);
	}

	// Cross-product axes
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			glm::vec3 axis = glm::cross(aAxis[i], bAxis[j]);

			const float axisLenSq = glm::dot(axis, axis);

			if (axisLenSq < 1e-8f)
				continue;

			axis = glm::normalize(axis);

			const int i1 = (i + 1) % 3;
			const int i2 = (i + 2) % 3;
			const int j1 = (j + 1) % 3;
			const int j2 = (j + 2) % 3;

			const float ra =
				aH[i1] * AbsR[i2][j] +
				aH[i2] * AbsR[i1][j];

			const float rb =
				bH[j1] * AbsR[i][j2] +
				bH[j2] * AbsR[i][j1];

			const float dist =
				std::fabs(t[i2] * R[i1][j] - t[i1] * R[i2][j]);

			const float overlap = ra + rb - dist;

			if (overlap < 0.0f)
				return false;

			UpdateMin(axis, overlap);
		}
	}

	glm::vec3 normal = minAxis;

	if (glm::dot(normal, tWorld) < 0.0f)
		normal = -normal;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = minOverlap;
	outEvent.collisionPoint = 0.5f * (aTransform.getPosition() + bTransform.getPosition());

	return true;
}

bool CuboidCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const Transform& boxTransform = _transform;
	const glm::vec3 halfExtents = GetHalfExtents() * GetAbsScale(boxTransform);

	const Transform& cylinderTransform = cylinder.GetTransform();
	const glm::vec3 cylinderScale = GetAbsScale(cylinderTransform);

	const float radius = cylinder.GetRadius() * std::max(cylinderScale.x, cylinderScale.z);
	const float halfHeight = (cylinder.GetHeight() * 0.5f) * cylinderScale.y;
	const glm::vec3 axis = glm::normalize(ToWorldDirNoScale(cylinderTransform, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 center = cylinderTransform.getPosition();

	const glm::vec3 segStart = center - axis * halfHeight;
	const glm::vec3 segEnd = center + axis * halfHeight;

	const glm::vec3 localStart = ToLocalNoScale(boxTransform, segStart);
	const glm::vec3 localEnd = ToLocalNoScale(boxTransform, segEnd);

	glm::vec3 closestSeg;
	glm::vec3 closestBox;
	const float distSq = ClosestPointSegmentAABB(localStart, localEnd, halfExtents, closestSeg, closestBox);
	if (IsContainer() && distSq > radius * radius)
	{
		outEvent.isColliding = true;

		const float dist = std::sqrt(std::max(distSq, 1e-8f));

		const glm::vec3 localNormal =
			(dist > 1e-5f)
			? (closestSeg - closestBox) / dist
			: glm::vec3(0.0f, 1.0f, 0.0f);

		const glm::vec3 worldNormal =
			glm::normalize(ToWorldDirNoScale(boxTransform, localNormal));

		// Push back inside container
		outEvent.collisionNormal = -worldNormal;

		// Amount outside allowed boundary
		outEvent.penetrationDepth = dist - radius;

		// Contact point on inner wall
		outEvent.collisionPoint = ToWorldNoScale(boxTransform, closestBox);

		return true;
	}

	if (distSq > radius * radius)
	{
		return false;
	}

	const float dist = std::sqrt(std::max(distSq, 1e-8f));
	const glm::vec3 localNormal = (dist > 1e-5f) ? (closestSeg - closestBox) / dist : glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 worldNormal = glm::normalize(ToWorldDirNoScale(boxTransform, localNormal));

	outEvent.isColliding = true;
	outEvent.collisionNormal = worldNormal;
	outEvent.penetrationDepth = radius - dist;
	outEvent.collisionPoint = ToWorldNoScale(boxTransform, closestBox);
	return true;
}

bool CuboidCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const Transform& boxTransform = _transform;
	const glm::vec3 halfExtents = GetHalfExtents() * GetAbsScale(boxTransform);

	const Transform& capsuleTransform = capsule.GetTransform();
	const glm::vec3 capsuleScale = GetAbsScale(capsuleTransform);

	const float radius = capsule.GetRadius() * std::max(capsuleScale.x, capsuleScale.z);
	const float halfHeight = (capsule.GetHeight() * 0.5f) * capsuleScale.y;
	const float segmentHalf = std::max(0.0f, halfHeight - radius);

	const glm::vec3 axis = glm::normalize(ToWorldDirNoScale(capsuleTransform, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 center = capsuleTransform.getPosition();

	const glm::vec3 segStart = center - axis * segmentHalf;
	const glm::vec3 segEnd = center + axis * segmentHalf;

	const glm::vec3 localStart = ToLocalNoScale(boxTransform, segStart);
	const glm::vec3 localEnd = ToLocalNoScale(boxTransform, segEnd);

	glm::vec3 closestSeg;
	glm::vec3 closestBox;
	const float distSq = ClosestPointSegmentAABB(localStart, localEnd, halfExtents, closestSeg, closestBox);
	if (IsContainer() && distSq > radius * radius)
	{
		outEvent.isColliding = true;

		const float dist = std::sqrt(std::max(distSq, 1e-8f));

		const glm::vec3 localNormal =
			(dist > 1e-5f)
			? (closestSeg - closestBox) / dist
			: glm::vec3(0.0f, 1.0f, 0.0f);

		const glm::vec3 worldNormal =
			glm::normalize(ToWorldDirNoScale(boxTransform, localNormal));

		// Push object back inside
		outEvent.collisionNormal = -worldNormal;

		// Amount beyond container boundary
		outEvent.penetrationDepth = dist - radius;

		// Contact point on inner wall
		outEvent.collisionPoint = ToWorldNoScale(boxTransform, closestBox);

		return true;
	}

	if (distSq > radius * radius)
	{
		return false;
	}

	const float dist = std::sqrt(std::max(distSq, 1e-8f));
	const glm::vec3 localNormal = (dist > 1e-5f) ? (closestSeg - closestBox) / dist : glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 worldNormal = glm::normalize(ToWorldDirNoScale(boxTransform, localNormal));

	outEvent.isColliding = true;
	outEvent.collisionNormal = worldNormal;
	outEvent.penetrationDepth = radius - dist;
	outEvent.collisionPoint = ToWorldNoScale(boxTransform, closestBox);
	return true;
}
#include "pch.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"
#include "CuboidCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include <algorithm>
#include <cmath>

bool PlaneCollider::IsInside(const glm::vec3& point) const
{
	const glm::vec3 worldNormal = glm::normalize(ToWorldDirNoScale(_transform, _Normal));
	const float d = glm::dot(worldNormal, point - _transform.getPosition());
	return d <= 0.0f;
}

bool PlaneCollider::Intersects(const Line&) const
{
	return false;
}

float PlaneCollider::ShortestDistanceToPoint(const glm::vec3& point) const
{
	const glm::vec3 n = glm::normalize(_Normal);
	return std::fabs(glm::dot(n, point - _transform.getPosition()));
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
	const glm::vec3 n = glm::normalize(ToWorldDirNoScale(_transform, _Normal));
	const float sphereRadius = sphere.GetRadius() * GetMaxAbsScale(sphere.GetTransform());
	const float signedDistance = glm::dot(n, sphere.GetTransform().getPosition() - _transform.getPosition());
	const float absDistance = std::fabs(signedDistance);
	if (IsContainer())
	{
		if (absDistance < sphereRadius)
		{
			return false; // inside is fine
		}

		const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

		outEvent.isColliding = true;
		outEvent.collisionNormal = -normal; // push inward
		outEvent.penetrationDepth = absDistance - sphereRadius;
		outEvent.collisionPoint = sphere.GetTransform().getPosition() - normal * sphereRadius;
		return true;
	}


	if (absDistance > sphereRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = sphereRadius - absDistance;
	outEvent.collisionPoint = sphere.GetTransform().getPosition() - normal * sphereRadius;
	return true;
}

bool PlaneCollider::CollideWithPlane(const PlaneCollider&, CollisionEvent&) const
{
	return false;
}

bool PlaneCollider::CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(ToWorldDirNoScale(_transform, _Normal));
	const Transform& cuboidTransform = cuboid.GetTransform();
	const glm::vec3 halfExtents = cuboid.GetHalfExtents() * GetAbsScale(cuboidTransform);

	const glm::vec3 axisX = glm::normalize(ToWorldDirNoScale(cuboidTransform, glm::vec3(1.0f, 0.0f, 0.0f)));
	const glm::vec3 axisY = glm::normalize(ToWorldDirNoScale(cuboidTransform, glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::vec3 axisZ = glm::normalize(ToWorldDirNoScale(cuboidTransform, glm::vec3(0.0f, 0.0f, 1.0f)));

	const float projectedRadius =
		halfExtents.x * std::fabs(glm::dot(n, axisX)) +
		halfExtents.y * std::fabs(glm::dot(n, axisY)) +
		halfExtents.z * std::fabs(glm::dot(n, axisZ));

	const float signedDistance = glm::dot(n, cuboidTransform.getPosition() - _transform.getPosition());
	const float absDistance = std::fabs(signedDistance);
	if (IsContainer())
	{
		if (absDistance < projectedRadius)
		{
			return false; // inside is valid
		}

		const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

		outEvent.isColliding = true;
		outEvent.collisionNormal = -normal; // push back inside
		outEvent.penetrationDepth = absDistance - projectedRadius;
		outEvent.collisionPoint = cuboidTransform.getPosition() - normal * projectedRadius;

		return true;
	}


	if (absDistance > projectedRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = projectedRadius - absDistance;
	outEvent.collisionPoint = cuboidTransform.getPosition() - normal * absDistance;
	return true;
}

bool PlaneCollider::CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(ToWorldDirNoScale(_transform, _Normal));
	const Transform& cylinderTransform = cylinder.GetTransform();
	const glm::vec3 localNormal = ToLocalDirNoScale(cylinderTransform, n);
	const glm::vec3 scale = GetAbsScale(cylinderTransform);

	const float radius = cylinder.GetRadius() * std::max(scale.x, scale.z);
	const float halfHeight = (cylinder.GetHeight() * 0.5f) * scale.y;

	const float projectedRadius =
		radius * std::sqrt((localNormal.x * localNormal.x) + (localNormal.z * localNormal.z)) +
		halfHeight * std::fabs(localNormal.y);

	const float signedDistance = glm::dot(n, cylinderTransform.getPosition() - _transform.getPosition());
	const float absDistance = std::fabs(signedDistance);
	if (IsContainer() && absDistance > projectedRadius)
	{
		outEvent.isColliding = true;

		const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

		outEvent.collisionNormal = -normal; // push inward
		outEvent.penetrationDepth = absDistance - projectedRadius; // always positive
		outEvent.collisionPoint = cylinderTransform.getPosition() - normal * projectedRadius;

		return true;
	}


	if (absDistance > projectedRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = projectedRadius - absDistance;
	outEvent.collisionPoint = cylinderTransform.getPosition() - normal * absDistance;
	return true;
}

bool PlaneCollider::CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const
{
	const glm::vec3 n = glm::normalize(ToWorldDirNoScale(_transform, _Normal));
	const Transform& capsuleTransform = capsule.GetTransform();
	const glm::vec3 localNormal = ToLocalDirNoScale(capsuleTransform, n);
	const glm::vec3 scale = GetAbsScale(capsuleTransform);

	const float radius = capsule.GetRadius() * std::max(scale.x, scale.z);
	const float halfHeight = (capsule.GetHeight() * 0.5f) * scale.y;
	const float segmentHalf = std::max(0.0f, halfHeight - radius);

	const float projectedRadius = segmentHalf * std::fabs(localNormal.y) + radius;

	const float signedDistance = glm::dot(n, capsuleTransform.getPosition() - _transform.getPosition());
	const float absDistance = std::fabs(signedDistance);
	if (IsContainer())
	{
		if (absDistance <= projectedRadius)
		{
			return false; // still inside container
		}

		outEvent.isColliding = true;

		const glm::vec3 normal = (signedDistance >= 0.0f) ? -n : n;
		outEvent.collisionNormal = normal;
		outEvent.penetrationDepth = absDistance - projectedRadius;
		outEvent.collisionPoint = capsuleTransform.getPosition() - normal * projectedRadius;

		return true;
	}


	if (absDistance > projectedRadius)
	{
		return false;
	}

	const glm::vec3 normal = (signedDistance >= 0.0f) ? n : -n;

	outEvent.isColliding = true;
	outEvent.collisionNormal = normal;
	outEvent.penetrationDepth = projectedRadius - absDistance;
	outEvent.collisionPoint = capsuleTransform.getPosition() - normal * absDistance;
	return true;
}
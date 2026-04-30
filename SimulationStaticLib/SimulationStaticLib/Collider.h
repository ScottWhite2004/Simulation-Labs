#pragma once
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Line.h"
#include "CollisionEvent.h"
#include "Transform.h"

class SphereCollider;
class PlaneCollider;
class CuboidCollider;
class CylinderCollider;
class CapsuleCollider;

class Collider
{
protected:
	Transform _transform;
	bool _isContainer{ false };
	static glm::vec3 AbsVec3(const glm::vec3& value)
	{
		return glm::vec3(std::fabs(value.x), std::fabs(value.y), std::fabs(value.z));
	}

	static glm::vec3 GetAbsScale(const Transform& transform)
	{
		return AbsVec3(transform.getScale());
	}

	static float GetMaxAbsScale(const Transform& transform)
	{
		const glm::vec3 scale = GetAbsScale(transform);
		return std::max(scale.x, std::max(scale.y, scale.z));
	}

	static glm::vec3 ToLocalNoScale(const Transform& transform, const glm::vec3& worldPoint)
	{
		const glm::quat invRotation = glm::inverse(transform.getRotation());
		return invRotation * (worldPoint - transform.getPosition());
	}

	static glm::vec3 ToLocalDirNoScale(const Transform& transform, const glm::vec3& worldDir)
	{
		const glm::quat invRotation = glm::inverse(transform.getRotation());
		return invRotation * worldDir;
	}

	static glm::vec3 ToWorldNoScale(const Transform& transform, const glm::vec3& localPoint)
	{
		return transform.getPosition() + (transform.getRotation() * localPoint);
	}

	static glm::vec3 ToWorldDirNoScale(const Transform& transform, const glm::vec3& localDir)
	{
		return transform.getRotation() * localDir;
	}

public:
	virtual ~Collider() = default;
	Collider(Transform transform) : _transform(transform) {}

	virtual bool IsInside(const glm::vec3& point) const { return false; }
	virtual bool Intersects(const Line& line) const { return false; }

	virtual bool Collide(const Collider& other, CollisionEvent& outEvent) const = 0;
	virtual bool CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const { return false; }

	virtual glm::vec3 calculateLocalInertiaTensor(float mass) const = 0;

	void SetTransform(const Transform& newTransform) { _transform = newTransform; }
	Transform GetTransform() const { return _transform; }

	void SetContainer(bool isContainer) { _isContainer = isContainer; }
	bool IsContainer() const { return _isContainer; }
};


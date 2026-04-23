#pragma once
#include "Collider.h"
#include "glm/glm.hpp"

class SphereCollider;
class PlaneCollider;
class CuboidCollider;
class CylinderCollider;

class CapsuleCollider : public Collider
{
	float _radius{ 0.5f };
	float _height{ 1.0f };

public:
	CapsuleCollider() = default;
	CapsuleCollider(const glm::vec3& position, float radius, float height)
		: _radius(radius), _height(height)
	{
		_Position = position;
	}

	glm::vec3 calculateLocalInertiaTensor(float mass) const override;
	bool IsInside(const glm::vec3& point) const override;
	bool Intersects(const Line& line) const override;

	float GetRadius() const { return _radius; }
	float GetHeight() const { return _height; }
	float GetSegmentHalfLength() const
	{
		const float halfH = _height * 0.5f;
		return (halfH > _radius) ? (halfH - _radius) : 0.0f;
	}

	bool Collide(const Collider& other, CollisionEvent& outEvent) const override;
	bool CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const override;
	bool CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const override;
	bool CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const override;
	bool CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const override;
	bool CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const override;
};


#pragma once
#include "Collider.h"
#include "glm/glm.hpp"

class SphereCollider;
class PlaneCollider;
class CylinderCollider;
class CapsuleCollider;

class CuboidCollider : public Collider
{
	float _width{ 1.0f };
	float _height{ 1.0f };
	float _depth{ 1.0f };

public:
	CuboidCollider() = default;
	CuboidCollider(const glm::vec3& position, float width, float height, float depth)
		: _width(width), _height(height), _depth(depth)
	{
		_Position = position;
	}

	glm::vec3 calculateLocalInertiaTensor(float mass) const override;
	bool IsInside(const glm::vec3& point) const override;
	bool Intersects(const Line& line) const override;

	float GetWidth() const { return _width; }
	float GetHeight() const { return _height; }
	float GetDepth() const { return _depth; }
	glm::vec3 GetHalfExtents() const { return glm::vec3(_width * 0.5f, _height * 0.5f, _depth * 0.5f); }

	bool Collide(const Collider& other, CollisionEvent& outEvent) const override;
	bool CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const override;
	bool CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const override;
	bool CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const override;
	bool CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const override;
	bool CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const override;
};
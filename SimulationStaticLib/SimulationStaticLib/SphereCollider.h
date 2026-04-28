#pragma once
#include "Collider.h"
#include "glm/glm.hpp"

class PlaneCollider;
class CuboidCollider;
class CylinderCollider;
class CapsuleCollider;

class SphereCollider : public Collider
{
	float _Radius{ 0.5f };

public:
	SphereCollider(const Transform& transform, float radius) : Collider(transform), _Radius(radius) {};

	bool IsInside(const glm::vec3& point) const override;
	bool Intersects(const Line& line) const override;
	glm::vec3 calculateLocalInertiaTensor(float mass) const override;

	float GetRadius() const { return _Radius; }

	bool Collide(const Collider& other, CollisionEvent& outEvent) const override;
	bool CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const override;
	bool CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const override;
	bool CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const override;
	bool CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const override;
	bool CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const override;
};
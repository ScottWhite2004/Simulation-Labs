#pragma once
#include "Collider.h"
#include "glm/glm.hpp"

class SphereCollider;
class CuboidCollider;
class CylinderCollider;
class CapsuleCollider;

class PlaneCollider : public Collider
{
	glm::vec3 _Normal{ 0.0f, 1.0f, 0.0f };

public:
	PlaneCollider(const glm::vec3& position, const glm::vec3& normal)
		: _Normal(glm::normalize(normal))
	{
		_Position = position;
	}

	bool IsInside(const glm::vec3& point) const override;
	bool Intersects(const Line& line) const override;
	float ShortestDistanceToPoint(const glm::vec3& point) const;
	glm::vec3 calculateLocalInertiaTensor(float mass) const override;

	glm::vec3 GetNormal() const { return _Normal; }

	bool Collide(const Collider& other, CollisionEvent& outEvent) const override;
	bool CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const override;
	bool CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const override;
	bool CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const override;
	bool CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const override;
	bool CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const override;
};
#pragma once
#include <glm/glm.hpp>
#include "Line.h"
#include "CollisionEvent.h"

class SphereCollider;
class PlaneCollider;
class CuboidCollider;
class CylinderCollider;
class CapsuleCollider;

class Collider
{
protected:
	glm::vec3 _Position{ 0.0f };

public:
	virtual ~Collider() = default;

	virtual bool IsInside(const glm::vec3& point) const { return false; }
	virtual bool Intersects(const Line& line) const { return false; }

	virtual bool Collide(const Collider& other, CollisionEvent& outEvent) const = 0;
	virtual bool CollideWithSphere(const SphereCollider& sphere, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithPlane(const PlaneCollider& plane, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithCuboid(const CuboidCollider& cuboid, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithCylinder(const CylinderCollider& cylinder, CollisionEvent& outEvent) const { return false; }
	virtual bool CollideWithCapsule(const CapsuleCollider& capsule, CollisionEvent& outEvent) const { return false; }

	virtual glm::vec3 calculateLocalInertiaTensor(float mass) const = 0;

	void SetPosition(const glm::vec3& newPosition) { _Position = newPosition; }
	glm::vec3 GetPosition() const { return _Position; }
};


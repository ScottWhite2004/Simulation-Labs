#include "WorldObjectManager.h"
#include "Sphere.h"
#include "Cuboid.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Capsule.h"

#include "SphereCollider.h"
#include "CuboidCollider.h"
#include "PlaneCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"

WorldObject* WorldObjectManager::createWorldObject(
	const std::string& name,
	Shape* shape,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& velocity,
	float mass)
{
	shape->create();

	WorldObject* worldObject = new WorldObject(name);
	RigidBody* rigidBody = new RigidBody(position, rotation, velocity, mass);

	worldObject->addShape(shape);
	worldObject->addRigidBody(rigidBody);
	worldObject->syncTransform();

	if (_physicsWorld != nullptr) {
		_physicsWorld->addObject(rigidBody);
	}

	addWorldObject(worldObject);
	return worldObject;
}

WorldObject* WorldObjectManager::addSphere(
	const std::string& name,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scale,
	float radius,
	const Material& material,
	const glm::vec3& velocity,
	float mass)
{
	Shape* shape = new Sphere(position, rotation, scale, material, radius);
	WorldObject* worldObject = createWorldObject(name, shape, position, rotation, velocity, mass);

	Transform colliderTransform(position, scale, glm::quat(rotation));
	Collider* collider = new SphereCollider(colliderTransform, radius);
	worldObject->addCollider(collider);
	worldObject->getRigidBody()->SetCollider(collider);
	worldObject->getRigidBody()->CalculateInertia();

	return worldObject;
}

WorldObject* WorldObjectManager::addCuboid(
	const std::string& name,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scale,
	float width,
	float height,
	float depth,
	const Material& material,
	const glm::vec3& velocity,
	float mass)
{
	Shape* shape = new Cuboid(position, rotation, scale, material, width, height, depth);
	WorldObject* worldObject = createWorldObject(name, shape, position, rotation, velocity, mass);

	Transform colliderTransform(position, scale, glm::quat(rotation));
	Collider* collider = new CuboidCollider(colliderTransform, width, height, depth);
	worldObject->addCollider(collider);
	worldObject->getRigidBody()->SetCollider(collider);
	worldObject->getRigidBody()->CalculateInertia();

	return worldObject;
}

WorldObject* WorldObjectManager::addPlane(
	const std::string& name,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scale,
	float width,
	float depth,
	const Material& material,
	const glm::vec3& velocity,
	float mass)
{
	Shape* shape = new Plane(position, rotation, scale, material, width, depth);
	WorldObject* worldObject = createWorldObject(name, shape, position, rotation, velocity, mass);

	Transform colliderTransform(position, scale, glm::quat(rotation));
	const glm::vec3 normal = glm::normalize(glm::quat(rotation) * glm::vec3(0.0f, 1.0f, 0.0f));
	Collider* collider = new PlaneCollider(colliderTransform, normal);
	worldObject->addCollider(collider);
	worldObject->getRigidBody()->SetCollider(collider);

	worldObject->getRigidBody()->SetStatic(true);

	return worldObject;
}

WorldObject* WorldObjectManager::addCylinder(
	const std::string& name,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scale,
	float radius,
	float height,
	const Material& material,
	int segments,
	const glm::vec3& velocity,
	float mass)
{
	Shape* shape = new Cylinder(position, rotation, scale, material, radius, height, segments);
	WorldObject* worldObject = createWorldObject(name, shape, position, rotation, velocity, mass);

	Transform colliderTransform(position, scale, glm::quat(rotation));
	Collider* collider = new CylinderCollider(colliderTransform, radius, height);
	worldObject->addCollider(collider);
	worldObject->getRigidBody()->SetCollider(collider);
	worldObject->getRigidBody()->CalculateInertia();

	return worldObject;
}

WorldObject* WorldObjectManager::addCapsule(
	const std::string& name,
	const glm::vec3& position,
	const glm::vec3& rotation,
	const glm::vec3& scale,
	float radius,
	float height,
	const Material& material,
	const glm::vec3& velocity,
	float mass)
{
	Shape* shape = new Capsule(position, rotation, scale, material, radius, height);
	WorldObject* worldObject = createWorldObject(name, shape, position, rotation, velocity, mass);

	Transform colliderTransform(position, scale, glm::quat(rotation));
	Collider* collider = new CapsuleCollider(colliderTransform, radius, height);
	worldObject->addCollider(collider);
	worldObject->getRigidBody()->SetCollider(collider);
	worldObject->getRigidBody()->CalculateInertia();

	return worldObject;
}
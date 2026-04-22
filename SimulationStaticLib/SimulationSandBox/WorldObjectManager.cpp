#include "WorldObjectManager.h"
#include "WorldObjectManager.h"
#include "Sphere.h"
#include "Cuboid.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Capsule.h"

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
	return createWorldObject(name, shape, position, rotation, velocity, mass);
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
	return createWorldObject(name, shape, position, rotation, velocity, mass);
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
	return createWorldObject(name, shape, position, rotation, velocity, mass);
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
	return createWorldObject(name, shape, position, rotation, velocity, mass);
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
	return createWorldObject(name, shape, position, rotation, velocity, mass);
}
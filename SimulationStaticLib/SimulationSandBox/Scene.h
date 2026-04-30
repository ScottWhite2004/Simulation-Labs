#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Camera.h"
#include "MaterialInteraction.h"
#include "SphereSpawner.h"
#include "CylinderSpawner.h"
#include "CapsuleSpawner.h"
#include "CuboidSpawner.h"

class WorldObjectManager;
class Material;

struct SceneCamera
{
	std::string name;
	Camera camera;
};

struct SceneSpawner
{
	SpawnerType type{ SpawnerType::SphereSpawner };
	SphereSpawner sphere;
	CylinderSpawner cylinder;
	CapsuleSpawner capsule;
	CuboidSpawner cuboid;
};

struct SceneObject
{
	enum class ShapeType
	{
		Sphere,
		Plane,
		Capsule,
		Cylinder,
		Cuboid
	};

	std::string name;
	ShapeType shape{ ShapeType::Sphere };
	glm::vec3 position{ 0.0f };
	glm::vec3 rotation{ 0.0f };
	glm::vec3 scale{ 1.0f };
	glm::vec3 size{ 1.0f };
	float radius{ 0.5f };
	float height{ 1.0f };
	glm::vec3 linearVelocity{ 0.0f };
	glm::vec3 angularVelocity{ 0.0f };
	bool isStatic{ false };
};

struct Scene
{
	std::string name;
	std::string description;
	bool gravity_on{ true };
	std::vector<SceneCamera> cameras;
	std::vector<SceneSpawner> spawners;
	std::vector<SceneObject> objects;
	std::vector<MaterialInteraction> interactions;

	bool loadFromBinaryFile(const std::string& path);
	void createWorldObjects(WorldObjectManager& worldObjectManager, const Material& defaultMaterial) const;
};
#pragma once
#include <string>
#include <vector>
#include "Camera.h"
#include "MaterialInteraction.h"
#include "SphereSpawner.h"
#include "CylinderSpawner.h"
#include "CapsuleSpawner.h"
#include "CuboidSpawner.h"
#include "FlatBufferLoader.h"

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

struct Scene
{
	std::string name;
	std::string description;
	bool gravity_on{ true };
	std::vector<SceneCamera> cameras;
	std::vector<SceneSpawner> spawners;
	std::vector<MaterialInteraction> interactions;

	bool loadFromBinaryFile(const std::string& path);
};


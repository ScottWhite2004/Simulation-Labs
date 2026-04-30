#pragma once
#include <string>
#include "Vec3Range.h"
#include "FloatRange.h"
#include "FixedLocation.h"
#include "RandomBox.h"
#include "RandomSphere.h"
#include "RepeatingSpawn.h"
#include "SingleBurstSpawn.h"

enum class SpawnType
{
	SingleBurstSpawn,
	RepeatingSpawn
};

enum class SpawnLocation
{
	FixedLocation,
	RandomBox,
	RandomSphere
};

enum class SpawnerOwnerType
{ 
	ONE,
	TWO,
	THREE,
	FOUR,
	SEQUENTIAL
};

enum class SpawnerType
{ 
	SphereSpawner,
	CylinderSpawner,
	CapsuleSpawner,
	CuboidSpawner
};


#pragma once
struct BaseSpawner
{
	std::string name;
	float start_time;
	SpawnType spawn_type{ SpawnType::SingleBurstSpawn };
	SpawnLocation location{ SpawnLocation::FixedLocation };

	SingleBurstSpawn single_burst;
	RepeatingSpawn repeating_spawn;

	FixedLocation fixed_location;
	RandomBox random_box;
	RandomSphere random_sphere;

	Vec3Range linear_velocity;
	Vec3Range angular_velocity;

	std::string material;
	SpawnerOwnerType owner{ SpawnerOwnerType::ONE };
};


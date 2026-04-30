#define GLM_ENABLE_EXPERIMENTAL
#include "SpawnerSystem.h"
#include "glm/gtc/random.hpp"
#include "glm/gtx/quaternion.hpp"

SpawnerSystem::SpawnerSystem(const Scene& scene, WorldObjectManager& worldObjectManager, const Material& defaultMaterial)
	: _scene(&scene), _worldObjectManager(&worldObjectManager), _defaultMaterial(&defaultMaterial)
{
	reset();
}

void SpawnerSystem::reset()
{
	_activeSpawners.clear();
	for (const SceneSpawner& spawner : _scene->spawners)
	{
		RuntimeSpawner runtime;
		runtime.spawner = &spawner;
		runtime.next_spawn_time = GetBaseSpawner(spawner).start_time;
		_activeSpawners.push_back(runtime);
	}
}

const BaseSpawner& SpawnerSystem::GetBaseSpawner(const SceneSpawner& spawner) const
{
	switch (spawner.type)
	{
	case SpawnerType::CylinderSpawner:
		return spawner.cylinder.base;
	case SpawnerType::CapsuleSpawner:
		return spawner.capsule.base;
	case SpawnerType::CuboidSpawner:
		return spawner.cuboid.base;
	default:
		return spawner.sphere.base;
	}
}

float SpawnerSystem::SampleFloatRange(const FloatRange& range) const
{
	return glm::linearRand(range.min, range.max);
}

glm::vec3 SpawnerSystem::SampleVec3Range(const Vec3Range& range) const
{
	return glm::linearRand(range.min, range.max);
}

void SpawnerSystem::ResolveSpawnTransform(const BaseSpawner& baseSpawner, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) const
{
	switch (baseSpawner.location)
	{
	case SpawnLocation::FixedLocation:
	{
		const Transform& transform = baseSpawner.fixed_location.transform;
		position = transform.getPosition();
		rotation = glm::eulerAngles(transform.getRotation());
		scale = transform.getScale();
		break;
	}
	case SpawnLocation::RandomBox:
		position = glm::linearRand(baseSpawner.random_box.min, baseSpawner.random_box.max);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		break;
	case SpawnLocation::RandomSphere:
		position = baseSpawner.random_sphere.center + glm::ballRand(baseSpawner.random_sphere.radius);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		break;
	default:
		position = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);
		break;
	}
}

void SpawnerSystem::SpawnFromSpawner(RuntimeSpawner& runtime)
{
	const SceneSpawner& spawner = *runtime.spawner;
	const BaseSpawner& baseSpawner = GetBaseSpawner(spawner);

	glm::vec3 position(0.0f);
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(1.0f);
	ResolveSpawnTransform(baseSpawner, position, rotation, scale);

	const glm::vec3 linearVelocity = SampleVec3Range(baseSpawner.linear_velocity);
	const glm::vec3 angularVelocity = SampleVec3Range(baseSpawner.angular_velocity);

	const std::string nameBase = baseSpawner.name.empty() ? "Spawner" : baseSpawner.name;
	const std::string name = nameBase + "_" + std::to_string(runtime.spawn_count);

	WorldObject* created = nullptr;
	switch (spawner.type)
	{
	case SpawnerType::SphereSpawner:
	{
		const float radius = SampleFloatRange(spawner.sphere.radius_range);
		created = _worldObjectManager->addSphere(name, position, rotation, scale, radius, *_defaultMaterial, linearVelocity, 1.0f);
		break;
	}
	case SpawnerType::CylinderSpawner:
	{
		const float radius = SampleFloatRange(spawner.cylinder.radius_range);
		const float height = SampleFloatRange(spawner.cylinder.height_range);
		created = _worldObjectManager->addCylinder(name, position, rotation, scale, radius, height, *_defaultMaterial, 36, linearVelocity, 1.0f);
		break;
	}
	case SpawnerType::CapsuleSpawner:
	{
		const float radius = SampleFloatRange(spawner.capsule.radius_range);
		const float height = SampleFloatRange(spawner.capsule.height_range);
		created = _worldObjectManager->addCapsule(name, position, rotation, scale, radius, height, *_defaultMaterial, linearVelocity, 1.0f);
		break;
	}
	case SpawnerType::CuboidSpawner:
	{
		const glm::vec3 size = SampleVec3Range(spawner.cuboid.size_range);
		created = _worldObjectManager->addCuboid(name, position, rotation, scale, size.x, size.y, size.z, *_defaultMaterial, linearVelocity, 1.0f);
		break;
	}
	default:
		break;
	}

	if (created && created->getRigidBody())
	{
		created->getRigidBody()->setAngularVelocity(angularVelocity);
	}

	++runtime.spawn_count;
}

void SpawnerSystem::update(float deltaTime)
{
	for (RuntimeSpawner& runtime : _activeSpawners)
	{
		if (runtime.completed || !runtime.spawner)
		{
			continue;
		}

		runtime.elapsed += deltaTime;

		const BaseSpawner& baseSpawner = GetBaseSpawner(*runtime.spawner);
		if (runtime.elapsed < baseSpawner.start_time)
		{
			continue;
		}

		if (baseSpawner.spawn_type == SpawnType::SingleBurstSpawn)
		{
			if (runtime.spawn_count == 0)
			{
				for (uint32_t i = 0; i < baseSpawner.single_burst.count; ++i)
				{
					SpawnFromSpawner(runtime);
				}
				runtime.completed = true;
			}
			continue;
		}

		if (baseSpawner.repeating_spawn.max_count == 0)
		{
			runtime.completed = true;
			continue;
		}

		if (baseSpawner.repeating_spawn.interval <= 0.0f)
		{
			SpawnFromSpawner(runtime);
			runtime.completed = runtime.spawn_count >= baseSpawner.repeating_spawn.max_count;
			continue;
		}

		while (!runtime.completed && runtime.elapsed >= runtime.next_spawn_time)
		{
			SpawnFromSpawner(runtime);
			runtime.next_spawn_time += baseSpawner.repeating_spawn.interval;

			if (runtime.spawn_count >= baseSpawner.repeating_spawn.max_count)
			{
				runtime.completed = true;
			}
		}
	}
}
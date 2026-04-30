#pragma once
#include <vector>
#include <string>
#include "Scene.h"
#include "WorldObjectManager.h"
#include "Material.h"

class SpawnerSystem
{
	struct RuntimeSpawner
	{
		const SceneSpawner* spawner{ nullptr };
		float elapsed{ 0.0f };
		float next_spawn_time{ 0.0f };
		uint32_t spawn_count{ 0 };
		bool completed{ false };
	};

	const Scene* _scene{ nullptr };
	WorldObjectManager* _worldObjectManager{ nullptr };
	const Material* _defaultMaterial{ nullptr };
	std::vector<RuntimeSpawner> _activeSpawners;

	const BaseSpawner& GetBaseSpawner(const SceneSpawner& spawner) const;
	float SampleFloatRange(const FloatRange& range) const;
	glm::vec3 SampleVec3Range(const Vec3Range& range) const;
	void ResolveSpawnTransform(const BaseSpawner& baseSpawner, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) const;
	void SpawnFromSpawner(RuntimeSpawner& runtime);

public:
	SpawnerSystem(const Scene& scene, WorldObjectManager& worldObjectManager, const Material& defaultMaterial);

	void reset();
	void update(float deltaTime);
};
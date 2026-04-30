#pragma once
#include "Scene.h"
#include "Scene_generated.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "RotationEuler.h"
#include "Transform.h"
#include "Vec3Range.h"
#include "FloatRange.h"
#include <fstream>
#include <vector>
#include <string>

class FlatBufferLoader
{
private:
	glm::vec3 ToVec3(const Simulation::Vec3& value)
	{
		return glm::vec3(value.x(), value.y(), value.z());
	}

	RotationEuler ToRotationEuler(const Simulation::RotationEuler& value)
	{
		RotationEuler rotation;
		rotation.yaw = value.yaw();
		rotation.pitch = value.pitch();
		rotation.roll = value.roll();
		return rotation;
	}

	Transform ToTransform(const Simulation::Transform& value)
	{
		Transform transform;
		transform.setPosition(ToVec3(value.position()));

		const RotationEuler rotationEuler = ToRotationEuler(value.orientation());
		transform.setRotation(glm::quat(glm::vec3(rotationEuler.pitch, rotationEuler.yaw, rotationEuler.roll)));

		transform.setScale(ToVec3(value.scale()));
		return transform;
	}

	Vec3Range ToVec3Range(const Simulation::Vec3Range& value)
	{
		Vec3Range range;
		range.min = ToVec3(value.min());
		range.max = ToVec3(value.max());
		return range;
	}

	FloatRange ToFloatRange(const Simulation::FloatRange& value)
	{
		FloatRange range;
		range.min = value.min();
		range.max = value.max();
		return range;
	}

	SpawnType MapSpawnType(Simulation::SpawnType type)
	{
		switch (type)
		{
		case Simulation::SpawnType_RepeatingSpawn:
			return SpawnType::RepeatingSpawn;
		case Simulation::SpawnType_SingleBurstSpawn:
		case Simulation::SpawnType_NONE:
		default:
			return SpawnType::SingleBurstSpawn;
		}
	}

	SpawnLocation MapSpawnLocation(Simulation::SpawnLocation location)
	{
		switch (location)
		{
		case Simulation::SpawnLocation_RandomBox:
			return SpawnLocation::RandomBox;
		case Simulation::SpawnLocation_RandomSphere:
			return SpawnLocation::RandomSphere;
		case Simulation::SpawnLocation_FixedLocation:
		case Simulation::SpawnLocation_NONE:
		default:
			return SpawnLocation::FixedLocation;
		}
	}

	SpawnerOwnerType MapSpawnerOwner(Simulation::SpawnerOwnerType type)
	{
		switch (type)
		{
		case Simulation::SpawnerOwnerType_TWO:
			return SpawnerOwnerType::TWO;
		case Simulation::SpawnerOwnerType_THREE:
			return SpawnerOwnerType::THREE;
		case Simulation::SpawnerOwnerType_FOUR:
			return SpawnerOwnerType::FOUR;
		case Simulation::SpawnerOwnerType_SEQUENTIAL:
			return SpawnerOwnerType::SEQUENTIAL;
		case Simulation::SpawnerOwnerType_ONE:
		default:
			return SpawnerOwnerType::ONE;
		}
	}

	SpawnerType MapSpawnerType(Simulation::SpawnerType type)
	{
		switch (type)
		{
		case Simulation::SpawnerType_CylinderSpawner:
			return SpawnerType::CylinderSpawner;
		case Simulation::SpawnerType_CapsuleSpawner:
			return SpawnerType::CapsuleSpawner;
		case Simulation::SpawnerType_CuboidSpawner:
			return SpawnerType::CuboidSpawner;
		case Simulation::SpawnerType_SphereSpawner:
		case Simulation::SpawnerType_NONE:
		default:
			return SpawnerType::SphereSpawner;
		}
	}

	BaseSpawner LoadBaseSpawner(const Simulation::BaseSpawner& baseSpawner)
	{
		BaseSpawner result;
		result.name = baseSpawner.name() ? baseSpawner.name()->str() : "";
		result.start_time = baseSpawner.start_time();
		result.spawn_type = MapSpawnType(baseSpawner.spawn_type_type());
		result.location = MapSpawnLocation(baseSpawner.location_type());
		result.owner = MapSpawnerOwner(baseSpawner.owner());
		result.material = baseSpawner.material() ? baseSpawner.material()->str() : "";

		if (const Simulation::Vec3Range* linear = baseSpawner.linear_velocity())
		{
			result.linear_velocity = ToVec3Range(*linear);
		}

		if (const Simulation::Vec3Range* angular = baseSpawner.angular_velocity())
		{
			result.angular_velocity = ToVec3Range(*angular);
		}

		if (result.spawn_type == SpawnType::RepeatingSpawn)
		{
			if (const Simulation::RepeatingSpawn* repeating = baseSpawner.spawn_type_as_RepeatingSpawn())
			{
				result.repeating_spawn.interval = repeating->interval();
				result.repeating_spawn.max_count = repeating->max_count();
			}
		}
		else
		{
			if (const Simulation::SingleBurstSpawn* burst = baseSpawner.spawn_type_as_SingleBurstSpawn())
			{
				result.single_burst.count = burst->count();
			}
		}

		switch (result.location)
		{
		case SpawnLocation::FixedLocation:
			if (const Simulation::FixedLocation* fixed = baseSpawner.location_as_FixedLocation())
			{
				if (const Simulation::Transform* transform = fixed->transform())
				{
					result.fixed_location.transform = ToTransform(*transform);
				}
			}
			break;
		case SpawnLocation::RandomBox:
			if (const Simulation::RandomBox* box = baseSpawner.location_as_RandomBox())
			{
				result.random_box.min = ToVec3(*box->min());
				result.random_box.max = ToVec3(*box->max());
			}
			break;
		case SpawnLocation::RandomSphere:
			if (const Simulation::RandomSphere* sphere = baseSpawner.location_as_RandomSphere())
			{
				result.random_sphere.center = ToVec3(*sphere->center());
				result.random_sphere.radius = sphere->radius();
			}
			break;
		default:
			break;
		}

		return result;
	}

	SceneCamera LoadCamera(const Simulation::Camera& camera)
	{
		SceneCamera result;
		result.name = camera.name() ? camera.name()->str() : "Camera";

		glm::vec3 position(0.0f);
		glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);

		if (const Simulation::Transform* transform = camera.transform())
		{
			position = ToVec3(transform->position());

			const RotationEuler rotationEuler = ToRotationEuler(transform->orientation());
			rotation = glm::quat(glm::vec3(rotationEuler.pitch, rotationEuler.yaw, rotationEuler.roll));
		}

		const glm::vec3 forward = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3 up = rotation * glm::vec3(0.0f, 1.0f, 0.0f);

		float fov = 90.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		float orthoHeight = 10.0f;
		ProjectionType projection = ProjectionType::Perspective;

		if (const Simulation::PerspectiveCamera* perspective = camera.camera_type_as_PerspectiveCamera())
		{
			fov = perspective->fov();
			nearPlane = perspective->near();
			farPlane = perspective->far();
			projection = ProjectionType::Perspective;
		}
		else if (const Simulation::OrthographicCamera* orthographic = camera.camera_type_as_OrthographicCamera())
		{
			orthoHeight = orthographic->size();
			nearPlane = orthographic->near();
			farPlane = orthographic->far();
			projection = ProjectionType::Orthographic;
		}

		result.camera = Camera(position, position + forward, up, fov, 16.0f / 9.0f, nearPlane, farPlane, orthoHeight, projection);
		return result;
	}

public:
	bool loadSceneFromFile(const std::string& path, Scene& scene);
};
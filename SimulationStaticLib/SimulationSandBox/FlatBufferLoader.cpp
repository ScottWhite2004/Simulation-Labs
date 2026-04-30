#include "FlatBufferLoader.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace
{
	std::string DumpHex(const std::vector<uint8_t>& buffer, size_t maxBytes)
	{
		std::ostringstream stream;
		const size_t count = std::min(maxBytes, buffer.size());

		stream << std::hex << std::setfill('0');
		for (size_t i = 0; i < count; ++i)
		{
			stream << std::setw(2) << static_cast<int>(buffer[i]);
			if (i + 1 < count)
			{
				stream << " ";
			}
		}

		return stream.str();
	}
}

bool FlatBufferLoader::loadSceneFromFile(const std::string& path, Scene& scene)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file.is_open())
	{
		return false;
	}

	const std::streamsize size = file.tellg();
	if (size <= 0)
	{
		return false;
	}

	std::vector<uint8_t> buffer(static_cast<size_t>(size));
	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(buffer.data()), size);

	flatbuffers::Verifier verifier(buffer.data(), buffer.size());
	if (!Simulation::VerifySizePrefixedSceneBuffer(verifier))
	{
		return false;
	}

	const Simulation::Scene* loadedScene = Simulation::GetScene(buffer.data());
	if (!loadedScene)
	{
		return false;
	}

	scene.name = loadedScene->name() ? loadedScene->name()->str() : "";
	scene.description = loadedScene->description() ? loadedScene->description()->str() : "";
	scene.gravity_on = loadedScene->gravity_on();

	scene.cameras.clear();
	scene.spawners.clear();
	scene.interactions.clear();

	if (const auto* sceneCameras = loadedScene->cameras())
	{
		for (flatbuffers::uoffset_t i = 0; i < sceneCameras->size(); ++i)
		{
			const Simulation::Camera* camera = sceneCameras->Get(i);
			if (camera != nullptr)
			{
				scene.cameras.push_back(LoadCamera(*camera));
			}
		}
	}

	const auto* spawnerTypes = loadedScene->spawners_type();
	const auto* spawnerData = loadedScene->spawners();
	if (spawnerTypes && spawnerData && spawnerTypes->size() == spawnerData->size())
	{
		for (flatbuffers::uoffset_t i = 0; i < spawnerData->size(); ++i)
		{
			SceneSpawner spawner;
			const Simulation::SpawnerType type = spawnerTypes->GetEnum<Simulation::SpawnerType>(i);
			spawner.type = MapSpawnerType(type);

			switch (type)
			{
			case Simulation::SpawnerType_SphereSpawner:
			{
				const Simulation::SphereSpawner* fb = reinterpret_cast<const Simulation::SphereSpawner*>(spawnerData->Get(i));
				if (fb && fb->base())
				{
					spawner.sphere.base = LoadBaseSpawner(*fb->base());
					if (const Simulation::FloatRange* range = fb->radius_range())
					{
						spawner.sphere.radius_range = ToFloatRange(*range);
					}
				}
				break;
			}
			case Simulation::SpawnerType_CylinderSpawner:
			{
				const Simulation::CylinderSpawner* fb = reinterpret_cast<const Simulation::CylinderSpawner*>(spawnerData->Get(i));
				if (fb && fb->base())
				{
					spawner.cylinder.base = LoadBaseSpawner(*fb->base());
					if (const Simulation::FloatRange* range = fb->radius_range())
					{
						spawner.cylinder.radius_range = ToFloatRange(*range);
					}
					if (const Simulation::FloatRange* range = fb->height_range())
					{
						spawner.cylinder.height_range = ToFloatRange(*range);
					}
				}
				break;
			}
			case Simulation::SpawnerType_CapsuleSpawner:
			{
				const Simulation::CapsuleSpawner* fb = reinterpret_cast<const Simulation::CapsuleSpawner*>(spawnerData->Get(i));
				if (fb && fb->base())
				{
					spawner.capsule.base = LoadBaseSpawner(*fb->base());
					if (const Simulation::FloatRange* range = fb->radius_range())
					{
						spawner.capsule.radius_range = ToFloatRange(*range);
					}
					if (const Simulation::FloatRange* range = fb->height_range())
					{
						spawner.capsule.height_range = ToFloatRange(*range);
					}
				}
				break;
			}
			case Simulation::SpawnerType_CuboidSpawner:
			{
				const Simulation::CuboidSpawner* fb = reinterpret_cast<const Simulation::CuboidSpawner*>(spawnerData->Get(i));
				if (fb && fb->base())
				{
					spawner.cuboid.base = LoadBaseSpawner(*fb->base());
					if (const Simulation::Vec3Range* range = fb->size_range())
					{
						spawner.cuboid.size_range = ToVec3Range(*range);
					}
				}
				break;
			}
			default:
				break;
			}

			scene.spawners.push_back(spawner);
		}
	}

	if (const auto* sceneInteractions = loadedScene->interactions())
	{
		for (flatbuffers::uoffset_t i = 0; i < sceneInteractions->size(); ++i)
		{
			const Simulation::MaterialInteraction* interaction = sceneInteractions->Get(i);
			if (!interaction)
			{
				continue;
			}

			MaterialInteraction loaded;
			loaded.material_a = interaction->material_a() ? interaction->material_a()->str() : "";
			loaded.material_b = interaction->material_b() ? interaction->material_b()->str() : "";
			loaded.restitution = interaction->restitution();
			loaded.static_friction = interaction->static_friction();
			loaded.dynamic_friction = interaction->dynamic_friction();

			scene.interactions.push_back(loaded);
		}
	}

	return true;
}
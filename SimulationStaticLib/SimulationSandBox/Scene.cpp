#include "Scene.h"
#include "FlatBufferLoader.h"
#include "WorldObjectManager.h"
#include "WorldObject.h"

bool Scene::loadFromBinaryFile(const std::string& path)
{
	FlatBufferLoader loader;
	return loader.loadSceneFromFile(path, *this);
}

void Scene::createWorldObjects(WorldObjectManager& worldObjectManager, const Material& defaultMaterial) const
{ 
	for (const SceneObject& obj : objects)
	{
		WorldObject* created = nullptr;

		switch (obj.shape)
		{
		case SceneObject::ShapeType::Sphere:
			created = worldObjectManager.addSphere(
				obj.name, obj.position, obj.rotation, obj.scale, obj.radius, defaultMaterial, obj.linearVelocity, 1.0f);
			break;
		case SceneObject::ShapeType::Cuboid:
			created = worldObjectManager.addCuboid(
				obj.name, obj.position, obj.rotation, obj.scale, obj.size.x, obj.size.y, obj.size.z, defaultMaterial, obj.linearVelocity, 1.0f);
			break;
		case SceneObject::ShapeType::Cylinder:
			created = worldObjectManager.addCylinder(
				obj.name, obj.position, obj.rotation, obj.scale, obj.radius, obj.height, defaultMaterial, 36, obj.linearVelocity, 1.0f);
			break;
		case SceneObject::ShapeType::Capsule:
			created = worldObjectManager.addCapsule(
				obj.name, obj.position, obj.rotation, obj.scale, obj.radius, obj.height, defaultMaterial, obj.linearVelocity, 1.0f);
			break;
		case SceneObject::ShapeType::Plane:
		{
			const float width = obj.size.x > 0.0f ? obj.size.x : 1.0f;
			const float depth = obj.size.z > 0.0f ? obj.size.z : 1.0f;
			created = worldObjectManager.addPlane(
				obj.name, obj.position, obj.rotation, obj.scale, width, depth, defaultMaterial, obj.linearVelocity, 1.0f);
			break;
		}
		default:
			break;
		}

		if (created && created->getRigidBody())
		{
			created->getRigidBody()->setAngularVelocity(obj.angularVelocity);
			if (obj.isStatic)
			{
				created->getRigidBody()->SetStatic(true);
			}
		}
	}
}

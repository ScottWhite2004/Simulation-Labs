#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "WorldObject.h"
#include "PhysicsWorld.h"
#include "Material.h"
#include <string>
#include <vector>

class WorldObjectManager
{
	std::vector<WorldObject*> _worldObjects;
	PhysicsWorld* _physicsWorld{ nullptr };

	WorldObject* createWorldObject(
		const std::string& name,
		Shape* shape,
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& velocity,
		float mass);

public:
	WorldObjectManager(PhysicsWorld* physicsWorld = nullptr) : _physicsWorld(physicsWorld) {}
	~WorldObjectManager() = default;

	void addWorldObject(WorldObject* worldObject) { _worldObjects.push_back(worldObject); }
	const std::vector<WorldObject*> getWorldObjects() const { return _worldObjects; }

	WorldObject* addSphere(
		const std::string& name,
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		float radius,
		const Material& material,
		const glm::vec3& velocity = glm::vec3(0.0f),
		float mass = 1.0f);

	WorldObject* addCuboid(
		const std::string& name,
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		float width,
		float height,
		float depth,
		const Material& material,
		const glm::vec3& velocity = glm::vec3(0.0f),
		float mass = 1.0f);

	WorldObject* addPlane(
		const std::string& name,
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		float width,
		float depth,
		const Material& material,
		const glm::vec3& velocity = glm::vec3(0.0f),
		float mass = 1.0f);

	WorldObject* addCylinder(
		const std::string& name,
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		float radius,
		float height,
		const Material& material,
		int segments = 36,
		const glm::vec3& velocity = glm::vec3(0.0f),
		float mass = 1.0f);

	WorldObject* addCapsule(
		const std::string& name,
		const glm::vec3& position,
		const glm::vec3& rotation,
		const glm::vec3& scale,
		float radius,
		float height,
		const Material& material,
		const glm::vec3& velocity = glm::vec3(0.0f),
		float mass = 1.0f);

	void syncWorldObjects() {
		for (WorldObject* obj : _worldObjects) {
			obj->syncTransform();
		}
	}

	void uploadWorldObjects(const VulkanContext& ctx, uint32_t framesInFlight, VkImageView textureImageView, VkSampler textureSampler, const std::vector<VkDescriptorBufferInfo>& lightinBufferInfos) {
		for (WorldObject* obj : _worldObjects) {
			if (obj->getShape()) {
				obj->getShape()->upload(ctx, framesInFlight, textureImageView, textureSampler, lightinBufferInfos);
			}
		}
	}

	void drawWorldObjects(VkCommandBuffer cmd, VkPipeline pipeline, VkPipelineLayout layout, uint32_t currentFrame) {
		for (WorldObject* obj : _worldObjects) {
			if (obj->getShape()) {
				obj->getShape()->draw(cmd, pipeline, layout, currentFrame);
			}
		}
	}

	void destroyWorldObjects(const VulkanContext& ctx) {
		for (WorldObject* obj : _worldObjects) {
			if (obj->getShape()) {
				obj->getShape()->destroy(ctx);
				delete obj->getShape();
			}
			if (obj->getRigidBody()) {
				if (_physicsWorld != nullptr)
				{
					_physicsWorld->removeObject(obj->getRigidBody());
				}
				delete obj->getRigidBody();
			}
			if(obj->getCollider()) {
				delete obj->getCollider();
			}
			delete obj;
		}
		_worldObjects.clear();
	}

	void updateUniformBuffers(uint32_t frameIndex, const glm::mat4& view, const glm::mat4& proj) {
		for (WorldObject* obj : _worldObjects) {
			if (obj->getShape()) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, obj->getShape()->getPos());

				const glm::quat rotationQuat = glm::normalize(obj->getShape()->getRot());
				model = model * glm::mat4_cast(rotationQuat);

				obj->getShape()->updateUniformBuffer(frameIndex, model, view, proj);
			}
		}
	}
};
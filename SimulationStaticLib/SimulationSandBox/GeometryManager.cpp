#include "GeometryManager.h"

#include <cmath>

#include "Capsule.h"
#include "Cuboid.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Sphere.h"

void GeometryManager::uploadShapes(const VulkanContext& ctx, uint32_t framesInFlight, VkImageView textureImageView, VkSampler textureSampler, const std::vector<VkDescriptorBufferInfo>& lightinBufferInfos)
{
	for (Shape* shape : _shapes) {
		if (shape == nullptr) {
			continue;
		}
		shape->upload(ctx, framesInFlight, textureImageView, textureSampler, lightinBufferInfos);
	}
}

void GeometryManager::drawShapes(VkCommandBuffer cmd, VkPipeline pipeline, VkPipelineLayout layout, uint32_t currentFrame)
{
	for (Shape* shape : _shapes) {
		if (shape == nullptr) {
			continue;
		}
		shape->draw(cmd, pipeline, layout, currentFrame);
	}
}

void GeometryManager::destroyShapes(const VulkanContext& ctx)
{
	for (Shape* shape : _shapes) {
		if (shape == nullptr) {
			continue;
		}

		shape->destroy(ctx);
		delete shape;
	}

	_shapes.clear();
}

void GeometryManager::addSphere(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float radius, const Material& material)
{
	Shape* sphere = new Sphere(position, rotation,scale,material, radius);
	sphere->create();
	addShape(sphere);
}

void GeometryManager::addCuboid(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float width, float height, float depth, const Material& material)
{
	const float colliderRadius = 0.5f * std::sqrt((width * width) + (height * height) + (depth * depth));
	Shape* cuboid = new Cuboid(position, rotation,scale, material, width, height, depth);
	cuboid->create();
	addShape(cuboid);
}

void GeometryManager::addPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float width, float depth,const Material& material)
{
	const float colliderRadius = 0.5f * std::sqrt((width * width) + (depth * depth));
	Shape* plane = new Plane(position, rotation, scale, material, width, depth);
	plane->create();
	addShape(plane);
}

void GeometryManager::addCylinder(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float radius, float height, const Material& material)
{
	const float halfHeight = 0.5f * height;
	const float colliderRadius = std::sqrt((radius * radius) + (halfHeight * halfHeight));

	const int defaultSegments = 36;
	Shape* cylinder = new Cylinder(position, rotation, scale, material, radius, height, defaultSegments);
	cylinder->create();
	addShape(cylinder);
}

void GeometryManager::addCapsule(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float radius, float height, const Material& material)
{
	const float colliderRadius = std::max(radius, 0.5f * height);
	Shape* capsule = new Capsule(position, rotation, scale,material, radius, height);
	capsule->create();
	addShape(capsule);
}

void GeometryManager::updateUniformBuffers(uint32_t frameIndex, const glm::mat4& view, const glm::mat4& proj)
{
	for (Shape* shape : _shapes) {
		if (shape == nullptr) {
			continue;
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, shape->getPos());

		const glm::quat rotationQuat = glm::normalize(shape->getRot());
		model = model * glm::mat4_cast(rotationQuat);

		shape->updateUniformBuffer(frameIndex, model, view, proj);
	}
}
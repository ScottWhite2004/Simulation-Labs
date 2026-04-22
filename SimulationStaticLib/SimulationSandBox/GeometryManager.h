#pragma once
#include <vector>
#include "Shape.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
class GeometryManager
{
	std::vector<Shape*> _shapes;

public:
	GeometryManager() = default;
	~GeometryManager() = default;
	void addShape(Shape* shape) { _shapes.push_back(shape); }
	std::vector<Shape*> getShapes() const { return _shapes; }
	void uploadShapes(const VulkanContext& ctx, uint32_t framesInFlight, VkImageView textureImageView, VkSampler textureSampler, const std::vector<VkDescriptorBufferInfo>& lightinBufferInfos);
	void drawShapes(VkCommandBuffer cmd, VkPipeline pipeline, VkPipelineLayout layout, uint32_t currentFrame);
	void destroyShapes(const VulkanContext& ctx);
	void updateUniformBuffers(uint32_t frameIndex, const glm::mat4& view, const glm::mat4& proj);
	Shape* getShape(size_t index) const { return _shapes[index]; }
	void addSphere(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float radius, const Material& material);
	void addCuboid(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float width, float height, float depth, const Material& material);
	void addPlane(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float width, float depth, const Material& material);
	void addCylinder(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float radius, float height, const Material& material);
	void addCapsule(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, float radius, float height, const Material& material);
};


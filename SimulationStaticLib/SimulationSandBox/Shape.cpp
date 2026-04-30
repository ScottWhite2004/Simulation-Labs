#include "Shape.h"
#include <span>


namespace
{
	uint32_t findMemoryType(VkPhysicalDevice phys, uint32_t typeFilter, VkMemoryPropertyFlags props)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(phys, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & props) == props) {
				return i;
			}
		}
		throw std::runtime_error("failed to find suitable memory type!");
	}

	void createBuffer(const VulkanContext& ctx, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(ctx.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(ctx.getDevice(), buffer, &memRequirements);
		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(ctx.getPhysicalDevice(), memRequirements.memoryTypeBits, properties);
		if (vkAllocateMemory(ctx.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}
		vkBindBufferMemory(ctx.getDevice(), buffer, bufferMemory, 0);
	}

	VkCommandBuffer beginSingleTimeCommands(const VulkanContext& ctx) {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = ctx.getCommandPool();
		allocInfo.commandBufferCount = 1;
		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(ctx.getDevice(), &allocInfo, &commandBuffer);
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	void endOneTime(const VulkanContext& ctx, VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;
		vkQueueSubmit(ctx.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(ctx.getGraphicsQueue());
		vkFreeCommandBuffers(ctx.getDevice(), ctx.getCommandPool(), 1, &commandBuffer);
	}

	void copyBuffer(const VulkanContext& ctx, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		const VkCommandBuffer commandBuffer = beginSingleTimeCommands(ctx);
		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
		endOneTime(ctx, commandBuffer);
	}
}

Shape::Shape() = default;

Shape::~Shape() = default;

Shape::Shape(Shape&& other)
	: GraphicsObjects(std::move(other)),
	_vertices(std::move(other._vertices)),
	_indices(std::move(other._indices)),
	_uniformBuffers(std::move(other._uniformBuffers)),
	_uniformBuffersMemory(std::move(other._uniformBuffersMemory)),
	_uniformBuffersMapped(std::move(other._uniformBuffersMapped)),
	_descriptorSets(std::move(other._descriptorSets)),
	_material(std::move(other._material)),
	_vertexBuffer(other._vertexBuffer),
	_indexBuffer(other._indexBuffer),
	_vertexBufferMemory(other._vertexBufferMemory),
	_indexBufferMemory(other._indexBufferMemory)
{
	other._vertexBuffer = VK_NULL_HANDLE;
	other._indexBuffer = VK_NULL_HANDLE;
	other._vertexBufferMemory = VK_NULL_HANDLE;
	other._indexBufferMemory = VK_NULL_HANDLE;
}

Shape& Shape::operator=(Shape&& other) {
	if (this != &other) {
		GraphicsObjects::operator=(std::move(other));
		_vertices = std::move(other._vertices);
		_indices = std::move(other._indices);
		_uniformBuffers = std::move(other._uniformBuffers);
		_uniformBuffersMemory = std::move(other._uniformBuffersMemory);
		_uniformBuffersMapped = std::move(other._uniformBuffersMapped);
		_descriptorSets = std::move(other._descriptorSets);
		_material = std::move(other._material);
		_vertexBuffer = other._vertexBuffer;
		_indexBuffer = other._indexBuffer;
		_vertexBufferMemory = other._vertexBufferMemory;
		_indexBufferMemory = other._indexBufferMemory;
		other._vertexBuffer = VK_NULL_HANDLE;
		other._indexBuffer = VK_NULL_HANDLE;
		other._vertexBufferMemory = VK_NULL_HANDLE;
		other._indexBufferMemory = VK_NULL_HANDLE;
	}
	return *this;
}

Shape::Shape(const Shape& other)
	: GraphicsObjects(other),
	_vertices(other._vertices),
	_indices(other._indices),
	_material(other._material)
{
}

Shape& Shape::operator=(const Shape& other) {
	if (this != &other) {
		_descriptorSets = {};
		_uniformBuffers = {};
		_uniformBuffersMemory = {};
		_uniformBuffersMapped = {};
		_vertexBuffer = VK_NULL_HANDLE;
		_vertexBufferMemory = VK_NULL_HANDLE;
		_indexBuffer = VK_NULL_HANDLE;
		_indexBufferMemory = VK_NULL_HANDLE;

		GraphicsObjects::operator=(other);
		_vertices = other._vertices;
		_indices = other._indices;
		_material = other._material;
	}
	return *this;
}

void Shape::upload(const VulkanContext& ctx, uint32_t framesInFlight, VkImageView textureImageView, VkSampler textureSampler, const std::vector<VkDescriptorBufferInfo>& lightingBufferInfos) {
	const VkDeviceSize vSize = sizeof(Vertex) * _vertices.size();
	VkBuffer vStaging{}; VkDeviceMemory vStageMem{};
	createBuffer(ctx, vSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vStaging, vStageMem);
	void* mapped = nullptr;
	vkMapMemory(ctx.getDevice(), vStageMem, 0, vSize, 0, &mapped);
	std::memcpy(mapped, _vertices.data(), static_cast<size_t>(vSize));
	vkUnmapMemory(ctx.getDevice(), vStageMem);

	createBuffer(ctx, vSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);
	copyBuffer(ctx, vStaging, _vertexBuffer, vSize);
	vkDestroyBuffer(ctx.getDevice(), vStaging, nullptr);
	vkFreeMemory(ctx.getDevice(), vStageMem, nullptr);

	// Index buffer
	const VkDeviceSize iSize = sizeof(uint16_t) * _indices.size();
	VkBuffer iStaging{}; VkDeviceMemory iStageMem{};
	createBuffer(ctx, iSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		iStaging, iStageMem);
	vkMapMemory(ctx.getDevice(), iStageMem, 0, iSize, 0, &mapped);
	std::memcpy(mapped, _indices.data(), static_cast<size_t>(iSize));
	vkUnmapMemory(ctx.getDevice(), iStageMem);

	createBuffer(ctx, iSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory);
	copyBuffer(ctx, iStaging, _indexBuffer, iSize);
	vkDestroyBuffer(ctx.getDevice(), iStaging, nullptr);
	vkFreeMemory(ctx.getDevice(), iStageMem, nullptr);

	// Per-frame UBOs
	const VkDeviceSize uboSize = sizeof(glm::mat4) * 3; // model, view, proj
	_uniformBuffers.resize(framesInFlight);
	_uniformBuffersMemory.resize(framesInFlight);
	_uniformBuffersMapped.resize(framesInFlight);

	for (uint32_t i = 0; i < framesInFlight; ++i) {
		createBuffer(ctx, uboSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_uniformBuffers[i], _uniformBuffersMemory[i]);
		vkMapMemory(ctx.getDevice(), _uniformBuffersMemory[i], 0, uboSize, 0, &_uniformBuffersMapped[i]);
	}

	// Descriptor sets (UBO + texture sampler)
	_descriptorSets.resize(framesInFlight);
	std::vector<VkDescriptorSetLayout> layouts(framesInFlight, ctx.getDescriptorSetLayout());

	VkDescriptorSetAllocateInfo alloc{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
	alloc.descriptorPool = ctx.getDescriptorPool();
	alloc.descriptorSetCount = framesInFlight;
	alloc.pSetLayouts = layouts.data();
	if (vkAllocateDescriptorSets(ctx.getDevice(), &alloc, _descriptorSets.data()) != VK_SUCCESS)
		throw std::runtime_error("Mesh: allocate descriptor sets failed");

	for (uint32_t i = 0; i < framesInFlight; ++i) {
		VkDescriptorBufferInfo bufInfo{};
		bufInfo.buffer = _uniformBuffers[i];
		bufInfo.offset = 0;
		bufInfo.range = uboSize;

		VkDescriptorImageInfo imgInfo{};
		imgInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imgInfo.imageView = textureImageView;
		imgInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 3> writesStorage{};
		std::span<VkWriteDescriptorSet, 3> writes{ writesStorage };

		writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[0].dstSet = _descriptorSets[i];
		writes[0].dstBinding = 0; // UBO
		writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writes[0].descriptorCount = 1;
		writes[0].pBufferInfo = &bufInfo;

		writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[1].dstSet = _descriptorSets[i];
		writes[1].dstBinding = 1; // sampler
		writes[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writes[1].descriptorCount = 1;
		writes[1].pImageInfo = &imgInfo;

		writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[2].dstSet = _descriptorSets[i];
		writes[2].dstBinding = 2; // lighting UBO
		writes[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writes[2].descriptorCount = 1;
		writes[2].pBufferInfo = &lightingBufferInfos[i];

		vkUpdateDescriptorSets(ctx.getDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
	}
}

void Shape::destroy(const VulkanContext& ctx) {
	for (size_t i = 0; i < _uniformBuffers.size(); ++i) {
		if (_uniformBuffersMapped[i]) vkUnmapMemory(ctx.getDevice(), _uniformBuffersMemory[i]);
		if (_uniformBuffers[i]) vkDestroyBuffer(ctx.getDevice(), _uniformBuffers[i], nullptr);
		if (_uniformBuffersMemory[i]) vkFreeMemory(ctx.getDevice(), _uniformBuffersMemory[i], nullptr);
	}
	_uniformBuffers.clear(); _uniformBuffersMemory.clear(); _uniformBuffersMapped.clear(); _descriptorSets.clear();

	if (_indexBuffer) vkDestroyBuffer(ctx.getDevice(), _indexBuffer, nullptr);
	if (_indexBufferMemory) vkFreeMemory(ctx.getDevice(), _indexBufferMemory, nullptr);
	if (_vertexBuffer) vkDestroyBuffer(ctx.getDevice(), _vertexBuffer, nullptr);
	if (_vertexBufferMemory) vkFreeMemory(ctx.getDevice(), _vertexBufferMemory, nullptr);

	_indexBuffer = _vertexBuffer = VK_NULL_HANDLE;
	_indexBufferMemory = _vertexBufferMemory = VK_NULL_HANDLE;
}

void Shape::updateUniformBuffer(uint32_t frameIndex, const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) const {
 if (frameIndex >= _uniformBuffersMapped.size() || !_uniformBuffersMapped[frameIndex]) {
		return;
	}
	struct UBO { alignas(16) glm::mat4 m, v, p; } const u{ model, view, proj };
	std::memcpy(_uniformBuffersMapped[frameIndex], &u, sizeof(u));
}

void Shape::draw(VkCommandBuffer cmd, VkPipeline pipeline, VkPipelineLayout layout,
	uint32_t currentFrame) {
	if (_vertices.empty() || _indices.empty()) return;
	if (_vertexBuffer == VK_NULL_HANDLE || _indexBuffer == VK_NULL_HANDLE) return;
	if (currentFrame >= _descriptorSets.size()) return;

	// Bind pipeline
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	// Bind vertex buffer
	std::array<VkDeviceSize, 1> offsetsStorage{ 0 };
	const std::span<VkDeviceSize, 1> offsets{ offsetsStorage };

	std::array<VkBuffer, 1> vbsStorage{ _vertexBuffer };
	const std::span < VkBuffer > vbs{ vbsStorage };
	vkCmdBindVertexBuffers(cmd, 0, 1, vbs.data(), offsets.data());

	// Bind index buffer (uint16_t indices in this code path)
	vkCmdBindIndexBuffer(cmd, _indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	// Bind per-frame descriptor set (set = 0)
	const VkDescriptorSet set = _descriptorSets[currentFrame];
	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout,
		0, 1, &set, 0, nullptr);

	// Issue draw
	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_indices.size()), 1, 0, 0, 0);
}





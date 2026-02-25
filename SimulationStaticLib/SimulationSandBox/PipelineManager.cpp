#include "PipelineManager.h"
void PipelineManager::createPipelineLayout(const std::vector<VkDescriptorSetLayout>& setLayouts, const std::vector<VkPushConstantRange>& pushConstants)
{
	if (_vulkanContext == nullptr) {
		throw std::runtime_error("PipelineManager: VulkanContext not initialized");
	}

	VkPipelineLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
	layoutInfo.pSetLayouts = setLayouts.empty() ? nullptr : setLayouts.data();
	layoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstants.size());
	layoutInfo.pPushConstantRanges = pushConstants.empty() ? nullptr : pushConstants.data();

	if (vkCreatePipelineLayout(_vulkanContext->getDevice(), &layoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

VkPipeline PipelineManager::createGraphicsPipeline(const std::string& name, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, const VkPipelineVertexInputStateCreateInfo* vertexInput, bool enableDepth, bool enableBlending)
{
	if (_vulkanContext == nullptr || _renderPassManager == nullptr) {
		throw std::runtime_error("PipelineManager: not initialized");
	}
	if (_pipelineLayout == VK_NULL_HANDLE) {
		throw std::runtime_error("PipelineManager: pipeline layout must be created before creating pipelines");
	}

	GraphicsPipelineBuilder builder;
	builder.setDevice(_vulkanContext->getDevice());
	builder.setPipelineLayout(_pipelineLayout);
	builder.setRenderPass(_renderPassManager->getRenderPass());
	builder.setShaderStages(vertShaderModule, fragShaderModule);

	// Vertex input - use provided or default empty
	if (vertexInput) {
		builder.setVertexInput(*vertexInput);
	}
	else {
		std::vector<VkVertexInputBindingDescription> emptyBindings;
		std::vector<VkVertexInputAttributeDescription> emptyAttribs;
		builder.setVertexInput(builder.MakeDefaultVertexInput(emptyBindings, emptyAttribs));
	}

	// Reasonable defaults
	builder.setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	builder.setRasterFill(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE);
	builder.setMultisample(VK_SAMPLE_COUNT_1_BIT);
	builder.setViewport(0.f, 0.f, static_cast<float>(_swapchainManager->getExtent().width), static_cast<float>(_swapchainManager->getExtent().height));
	builder.setScissor({ 0,0 }, _swapchainManager->getExtent());

	if (enableDepth) builder.enableDepthTest();
	else builder.disableDepth();

	builder.addColorBlendAttachment(enableBlending ? VK_TRUE : VK_FALSE);
	builder.setColorBlendLogic(VK_FALSE);

	VkPipeline pipeline = builder.build();
	_pipelines.emplace(name, pipeline);
	return pipeline;
}

VkPipeline PipelineManager::createGraphicsPipeline(const std::string& name, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, const VkPipelineVertexInputStateCreateInfo* vertexInput, bool enableDepth, bool enableBlending, VkPipelineLayout overrideLayout)
{
	if (_vulkanContext == nullptr || _renderPassManager == nullptr) {
		throw std::runtime_error("PipelineManager: not initialized");
	}
	if (_pipelineLayout == VK_NULL_HANDLE) {
		throw std::runtime_error("PipelineManager: pipeline layout must be created before creating pipelines");
	}

	GraphicsPipelineBuilder builder;
	builder.setDevice(_vulkanContext->getDevice());
	builder.setPipelineLayout(overrideLayout);
	builder.setRenderPass(_renderPassManager->getRenderPass());
	builder.setShaderStages(vertShaderModule, fragShaderModule);

	// Vertex input - use provided or default empty
	if (vertexInput) {
		builder.setVertexInput(*vertexInput);
	}
	else {
		std::vector<VkVertexInputBindingDescription> emptyBindings;
		std::vector<VkVertexInputAttributeDescription> emptyAttribs;
		builder.setVertexInput(builder.MakeDefaultVertexInput(emptyBindings, emptyAttribs));
	}

	// Reasonable defaults
	builder.setInputAssembly(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
	builder.setRasterFill(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
	builder.setMultisample(VK_SAMPLE_COUNT_1_BIT);
	builder.setViewport(0.f, 0.f, static_cast<float>(_swapchainManager->getExtent().width), static_cast<float>(_swapchainManager->getExtent().height));
	builder.setScissor({ 0,0 }, _swapchainManager->getExtent());

	if (enableDepth) builder.enableDepthTest();
	else builder.disableDepth();

	builder.addColorBlendAttachment(enableBlending ? VK_TRUE : VK_FALSE);
	builder.setColorBlendLogic(VK_FALSE);

	VkPipeline pipeline = builder.build();
	_pipelines.emplace(name, pipeline);
	return pipeline;
}

VkPipeline PipelineManager::getPipeline(const std::string& name) const
{
	auto it = _pipelines.find(name);
	if (it == _pipelines.end()) return VK_NULL_HANDLE;
	return it->second;
}

void PipelineManager::cleanup()
{
	if (_vulkanContext == nullptr) return;

	for (auto& kv : _pipelines) {
		if (kv.second != VK_NULL_HANDLE) {
			vkDestroyPipeline(_vulkanContext->getDevice(), kv.second, nullptr);
		}
	}
	_pipelines.clear();

	if (_pipelineLayout != VK_NULL_HANDLE) {
		vkDestroyPipelineLayout(_vulkanContext->getDevice(), _pipelineLayout, nullptr);
		_pipelineLayout = VK_NULL_HANDLE;
	}
}
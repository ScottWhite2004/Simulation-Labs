#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "VulkanContext.h"
#include "RenderPassManager.h"
#include "SwapChainManager.h"
#include "GraphicsPipelineBuilder.h"

class PipelineManager
{
	VulkanContext* _vulkanContext{ nullptr };
	RenderPassManager* _renderPassManager{ nullptr };
	SwapChainManager* _swapchainManager{ nullptr };

	VkPipelineLayout _pipelineLayout{ VK_NULL_HANDLE };
	std::unordered_map<std::string, VkPipeline> _pipelines;

public:
	PipelineManager() = default;
	~PipelineManager() = default;

	void initialize(VulkanContext* vulkanContext, RenderPassManager* renderPassManager, SwapChainManager* swapchainManager)
	{
		_vulkanContext = vulkanContext;
		_renderPassManager = renderPassManager;
		_swapchainManager = swapchainManager;
	}

	void createPipelineLayout(const std::vector<VkDescriptorSetLayout>& setLayouts, const std::vector<VkPushConstantRange>& pushConstants = {});

	VkPipeline createGraphicsPipeline(const std::string& name, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, const VkPipelineVertexInputStateCreateInfo* vertexInput, bool enableDepth, bool enableBlending);
	VkPipeline createGraphicsPipeline(const std::string& name, VkShaderModule vertShaderModule, VkShaderModule fragShaderModule, const VkPipelineVertexInputStateCreateInfo* vertexInput, bool enableDepth, bool enableBlending, VkPipelineLayout overrideLayout);

	VkPipeline getPipeline(const std::string& name) const;
	VkPipelineLayout getPipelineLayout() const { return _pipelineLayout; }

	void cleanup();
};


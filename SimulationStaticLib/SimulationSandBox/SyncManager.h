#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include "SwapChainManager.h"
class SyncManager
{
    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkSemaphore> _imagePresentSemaphores;
    std::vector<VkFence> _inFlightFences;

    std::vector<VkFence> _imagesInFlight;

    SwapChainManager* _swapchainManager{ nullptr };
    VulkanContext* _vulkanContext{ nullptr };

public:

    void initialize(VulkanContext* vulkanContext, SwapChainManager* swapChainManager, size_t maxFramesInFlight)
    {
        _vulkanContext = vulkanContext;
        _swapchainManager = swapChainManager;
        createSyncObjects(maxFramesInFlight);
        createPerImageSemaphores();
    }

    void createPerImageSemaphores()
    {
        size_t imageCount = _swapchainManager->getSwapChainImages().size();

        _imagePresentSemaphores.resize(_swapchainManager->getSwapChainImages().size());
        _imagesInFlight.resize(imageCount, VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        for (size_t i = 0; i < _swapchainManager->getSwapChainImages().size(); i++) {
            if (vkCreateSemaphore(_vulkanContext->getDevice(), &semaphoreInfo, nullptr, &_imagePresentSemaphores[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create semaphores for a swap chain image!");
            }
        }
    }

    void createSyncObjects(size_t maxFramesInFlight)
    {
        _imageAvailableSemaphores.resize(maxFramesInFlight);
        _renderFinishedSemaphores.resize(maxFramesInFlight);
        _inFlightFences.resize(maxFramesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < maxFramesInFlight; i++) {
            if (vkCreateSemaphore(_vulkanContext->getDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(_vulkanContext->getDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(_vulkanContext->getDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void waitForFrame(uint32_t frameIndex)
    {
        vkWaitForFences(_vulkanContext->getDevice(), 1, &_inFlightFences[frameIndex], VK_TRUE, UINT64_MAX);
    }

    void waitForImage(uint32_t imageIndex)
    {
        if (_imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(_vulkanContext->getDevice(), 1, &_imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
    }

    void markImageInFlight(uint32_t imageIndex, uint32_t frameIndex)
    {
        _imagesInFlight[imageIndex] = _inFlightFences[frameIndex];
    }

    void resetFrameFence(uint32_t frameIndex)
    {
        vkResetFences(_vulkanContext->getDevice(), 1, &_inFlightFences[frameIndex]);
    }

    std::vector<VkSemaphore>& getImageAvailableSemaphores() { return _imageAvailableSemaphores; }

    std::vector<VkSemaphore>& getRenderFinishedSemaphores() { return _renderFinishedSemaphores; }

    std::vector<VkSemaphore>& getImagePresentSemaphores() { return _imagePresentSemaphores; }

    std::vector<VkFence>& getInFlightFences() { return _inFlightFences; }

    void cleanup()
    {
        cleanupPerImageSemaphores();

        for (auto sem : _renderFinishedSemaphores)
        {
            vkDestroySemaphore(_vulkanContext->getDevice(), sem, nullptr);
        }
        _renderFinishedSemaphores.clear();

        for (auto sem : _imageAvailableSemaphores)
        {
            vkDestroySemaphore(_vulkanContext->getDevice(), sem, nullptr);
        }
        _imageAvailableSemaphores.clear();

        for (auto fence : _inFlightFences)
        {
            vkDestroyFence(_vulkanContext->getDevice(), fence, nullptr);
        }
        _inFlightFences.clear();
    }

    void cleanupPerImageSemaphores()
    {
        for (auto sem : _imagePresentSemaphores)
        {
            vkDestroySemaphore(_vulkanContext->getDevice(), sem, nullptr);
        }
        _imagePresentSemaphores.clear();
    }
};


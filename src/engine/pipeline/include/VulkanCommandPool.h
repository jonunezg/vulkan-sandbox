#pragma once

#include "Utilities.h"
#include "VulkanLogger.h"
#include "VulkanIncludes.h"
#include "VulkanPhysicalDevice.h"

class VulkanCommandPool
{
public:
    VulkanCommandPool(const VkDevice& logicalDevice, const PhysicalDevice& physicalDevice);
    ~VulkanCommandPool();

    VulkanCommandPool(VulkanCommandPool&) = delete;
    VulkanCommandPool(const VulkanCommandPool&) = delete;
    VulkanCommandPool(VulkanCommandPool&&) = delete;
    VulkanCommandPool(const VulkanCommandPool&&) = delete;

    void recordCommandBuffer(
        uint32_t imageIndex,
        VkRenderPass renderPass,
        const std::vector<VkFramebuffer>& framebuffers,
        VkExtent2D swapchainExtent,
        VkPipeline pipeline);

private:
    void createCommandBuffer();

    const VkDevice m_device;
    VkCommandPool m_commandPool;
    VkCommandBuffer m_commandBuffer;
};
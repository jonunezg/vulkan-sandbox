#pragma once

#include "Utilities.h"
#include "VulkanLogger.h"
#include "VulkanIncludes.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanVertexBuffer.h"

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
        size_t frameIndex,
        uint32_t imageIndex,
        VkRenderPass renderPass,
        const std::vector<VkFramebuffer>& framebuffers,
        VkExtent2D swapchainExtent,
        VkPipeline pipeline,
        const VulkanVertexBuffer& vertexBuffer);

    const VkCommandBuffer& getCommandBuffer(size_t index) const { return m_commandBuffers[index]; }

    const VkCommandPool& getCommandPool() { return m_commandPool; }

private:
    void createCommandBuffer();

    const VkDevice m_device;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
};
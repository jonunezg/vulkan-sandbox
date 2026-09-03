#pragma once

#include "Utilities.h"
#include "VulkanCommandBuffers.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"
#include "VulkanVertexBuffer.h"

class VulkanVertexCommandBuffers : VulkanCommandBuffers
{
public:
    VulkanVertexCommandBuffers(const VkDevice& device, const VkCommandPool& commandPool) :
    VulkanCommandBuffers { device, commandPool, 2 }
    {}

    void recordCommandBuffer(
    size_t frameIndex,
    uint32_t imageIndex,
    VkRenderPass renderPass,
    const std::vector<VkFramebuffer>& framebuffers,
    VkExtent2D swapchainExtent,
    VkPipeline pipeline,
    const VulkanVertexBuffer& vertexBuffer);

    const VkCommandBuffer& getCommandBuffer(uint32_t index) { return m_commandBuffers[index]; }
};
#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"

class VulkanCommandBuffers
{
public:
    VulkanCommandBuffers(const VkDevice& device, const VkCommandPool& commandPool, uint32_t commandBufferCount);

    VulkanCommandBuffers(VulkanCommandBuffers&) = delete;
    VulkanCommandBuffers(const VulkanCommandBuffers&) = delete;
    VulkanCommandBuffers(VulkanCommandBuffers&&) = delete;
    VulkanCommandBuffers(const VulkanCommandBuffers&&) = delete;

    ~VulkanCommandBuffers();

protected:
    const VkDevice& m_device;
    const VkCommandPool& m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
};
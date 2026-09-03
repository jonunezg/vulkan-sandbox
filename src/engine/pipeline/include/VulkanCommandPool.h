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

    const VkCommandPool& getCommandPool() { return m_commandPool; }

private:
    void createCommandBuffer();

    const VkDevice& m_device;
    VkCommandPool m_commandPool;
};
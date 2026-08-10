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

    void createCommandBuffer();
private:
    const VkDevice m_device;
    VkCommandPool m_commandPool;
};
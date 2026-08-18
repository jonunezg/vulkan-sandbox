#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanPhysicalDevice.h"

class VulkanLogicalDevice
{
public:
    VulkanLogicalDevice(const PhysicalDevice& physicalDevice);
    ~VulkanLogicalDevice();

    VulkanLogicalDevice(VulkanLogicalDevice&) = delete;
    VulkanLogicalDevice(const VulkanLogicalDevice&) = delete;
    VulkanLogicalDevice(VulkanLogicalDevice&&) = delete;
    VulkanLogicalDevice(const VulkanLogicalDevice&&) = delete;

    const VkDevice& getDevice() const { return m_device; }
    const VkQueue& getGraphicsQueue() const { return m_graphicsQueue; }
    const VkQueue& getPresentQueue() const { return m_presentQueue; }

private:
    VkDevice m_device = VK_NULL_HANDLE;

    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
};
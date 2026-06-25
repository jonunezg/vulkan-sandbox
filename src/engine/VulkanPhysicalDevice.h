#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance);
    ~VulkanPhysicalDevice();
private:
    std::vector<VkPhysicalDevice> getPhysicalDevices();

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    const std::shared_ptr<VulkanInstance> m_instance;
};
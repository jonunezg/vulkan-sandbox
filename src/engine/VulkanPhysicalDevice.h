#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"

struct PhysicalDevice
{
    VkPhysicalDevice device = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties properties = {};
    VkPhysicalDeviceFeatures features = {};
    std::vector<VkQueueFamilyProperties> queueFamilies = {};
};

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance);
    ~VulkanPhysicalDevice();
private:
    std::vector<PhysicalDevice> getPhysicalDevices();

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    const std::shared_ptr<VulkanInstance> m_instance;
};
#pragma once

#include <optional>
#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"

struct PhysicalDevice
{
    VkPhysicalDevice device = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties properties = {};
    VkPhysicalDeviceFeatures features = {};
    std::vector<VkQueueFamilyProperties> queueFamilies = {};
    std::optional<uint32_t> graphicQueueIndex;
};

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance);
    ~VulkanPhysicalDevice();

    const PhysicalDevice& getSelectedDevice() { return m_selectedDevice; }

private:
    std::vector<PhysicalDevice> getPhysicalDevices();

    PhysicalDevice m_selectedDevice;
    const std::shared_ptr<VulkanInstance> m_instance;
};
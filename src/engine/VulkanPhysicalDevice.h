#pragma once

#include <optional>
#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"

struct PhysicalDevice
{
    VkPhysicalDevice device = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties properties = {};
    VkPhysicalDeviceFeatures features = {};
    std::vector<VkQueueFamilyProperties> queueFamilies = {};
    std::optional<uint32_t> graphicQueueIndex;
    std::optional<uint32_t> presentQueueIndex;
};

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(
        const std::shared_ptr<VulkanInstance> instance,
        const std::shared_ptr<VulkanSurface> surface);
    ~VulkanPhysicalDevice();

    const PhysicalDevice& getSelectedDevice() { return m_selectedDevice; }

private:
    std::vector<PhysicalDevice> getPhysicalDevices();

    void processQueueFamilies(PhysicalDevice& device);

    PhysicalDevice m_selectedDevice;
    const std::shared_ptr<VulkanInstance> m_instance;
    const std::shared_ptr<VulkanSurface> m_surface;
};
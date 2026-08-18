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

    // Queue family indices
    std::optional<uint32_t> graphicQueueIndex;
    std::optional<uint32_t> presentQueueIndex;

    // Swapchain support
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanPhysicalDevice
{
public:
    VulkanPhysicalDevice(
        const VulkanInstance& instance,
        const VulkanSurface& surface);
    ~VulkanPhysicalDevice();

    const PhysicalDevice& getSelectedDevice();

private:
    const std::vector<PhysicalDevice> getPhysicalDevices() const;
    void UpdateSwapchainSupportSettings(const VkSurfaceKHR& surface, PhysicalDevice& storage) const;
    void processQueueFamilies(PhysicalDevice& device) const;

    PhysicalDevice m_selectedDevice;
    const VulkanInstance& m_instance;
    const VulkanSurface& m_surface;
};
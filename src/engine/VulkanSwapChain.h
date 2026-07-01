#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"

class VulkanSwapChain
{
public:
    VulkanSwapChain(
        const std::shared_ptr<VulkanPhysicalDevice> physicalDevice,
        const std::shared_ptr<VulkanLogicalDevice> logicalDevice,
        const std::shared_ptr<VulkanSurface> surface,
        const std::shared_ptr<WindowManager> windowManager);
    ~VulkanSwapChain();
private:

    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    const std::shared_ptr<VulkanPhysicalDevice> m_physicalDevice;
    const std::shared_ptr<VulkanLogicalDevice> m_logicalDevice;
    const std::shared_ptr<VulkanSurface> m_surface;
    const std::shared_ptr<WindowManager> m_windowManager;
    const VkSurfaceFormatKHR m_format;
    const VkExtent2D m_extent;

    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_images;
};
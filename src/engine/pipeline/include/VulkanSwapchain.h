#pragma once

#include "Utilities.h"
#include "VulkanImageView.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"

class VulkanSwapchain
{
public:
    VulkanSwapchain(
        const PhysicalDevice& physicalDevice,
        const std::shared_ptr<VulkanLogicalDevice> logicalDevice,
        const VkSurfaceKHR& surface,
        const WindowManager& windowManager);
    ~VulkanSwapchain();

    const VkExtent2D& getSwapchainExtent() const { return m_extent; }

    const VkFormat& getSwapchainFormat() const { return m_format.format; }

    const std::vector<VulkanImageView>& getImageViews() const { return m_imageViews; }

    const VkSwapchainKHR& getSwapchain() const { return m_swapchain; }

private:

    VkExtent2D selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createImageViews();

    const std::shared_ptr<VulkanLogicalDevice> m_logicalDevice;
    const WindowManager& m_windowManager;
    const VkSurfaceFormatKHR m_format;
    const VkExtent2D m_extent;

    VkSwapchainKHR m_swapchain;
    std::vector<VkImage> m_images;
    std::vector<VulkanImageView> m_imageViews;
};
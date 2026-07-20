#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogicalDevice.h"

class VulkanImageView
{
public:
    VulkanImageView(const std::shared_ptr<VulkanLogicalDevice> logicalDevice, const VkSurfaceFormatKHR& format, const VkImage& image);
    ~VulkanImageView();

    VulkanImageView(const VulkanImageView&) = delete;
    VulkanImageView(VulkanImageView&) = delete;
    VulkanImageView(const VulkanImageView&&) = delete;
    VulkanImageView(VulkanImageView&&) = delete;
private:
    std::shared_ptr<VulkanLogicalDevice> m_logicalDevice;
    VkImageView m_imageView;
};
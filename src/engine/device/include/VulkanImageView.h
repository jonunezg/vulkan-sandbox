#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogicalDevice.h"

class VulkanImageView
{
public:
    VulkanImageView(const VkDevice device, const VkSurfaceFormatKHR& format, const VkImage& image);
    ~VulkanImageView();

    VulkanImageView(const VulkanImageView&) = delete;
    VulkanImageView(VulkanImageView&) = delete;
    VulkanImageView(const VulkanImageView&&) = delete;
    VulkanImageView(VulkanImageView&& other) :
    m_device { other.m_device },
    m_imageView { other.m_imageView }
    {
        other.m_imageView = VK_NULL_HANDLE;
    }

    const VkImageView& getRawImageView() const { return m_imageView; }
private:
    const VkDevice m_device;
    VkImageView m_imageView;
};
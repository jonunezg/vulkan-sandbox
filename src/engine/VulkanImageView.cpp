#include "VulkanImageView.h"

VulkanImageView::VulkanImageView(const std::shared_ptr<VulkanLogicalDevice> logicalDevice, const VkSurfaceFormatKHR& format, const VkImage& image) :
m_logicalDevice { std::move(logicalDevice) }
{
    if (!m_logicalDevice)
    {
        throw std::runtime_error("Image view created without logical device");
    }

    const VkImageViewCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format.format,
        .components =
        {
            VK_COMPONENT_SWIZZLE_IDENTITY,  // r
            VK_COMPONENT_SWIZZLE_IDENTITY,  // g
            VK_COMPONENT_SWIZZLE_IDENTITY,  // b
            VK_COMPONENT_SWIZZLE_IDENTITY   // a
        },
        .subresourceRange =
        {
            VK_IMAGE_ASPECT_COLOR_BIT,  // aspect mask
            0,                          // base mip level
            1,                          // level count
            0,                          // base array layer
            1                           // layer count
        }
    };

    VK_TERMINATE_IF_FAILED(vkCreateImageView(m_logicalDevice->getDevice(), &createInfo, nullptr, &m_imageView));
}

VulkanImageView::~VulkanImageView()
{
    vkDestroyImageView(m_logicalDevice->getDevice(), m_imageView, nullptr);\
}
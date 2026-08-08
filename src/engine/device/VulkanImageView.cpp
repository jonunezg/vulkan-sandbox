#include "VulkanImageView.h"

VulkanImageView::VulkanImageView(const VkDevice device, const VkSurfaceFormatKHR& format, const VkImage& image) :
m_device { device }
{
    if (!m_device)
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

    VK_THROW_IF_FAILED(vkCreateImageView(m_device, &createInfo, nullptr, &m_imageView));

    std::cout << "Vulkan image view created: " << m_imageView << std::endl;
}

VulkanImageView::~VulkanImageView()
{
    if (m_imageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(m_device, m_imageView, nullptr);

        std::cout << "Vulkan image view destroyed: " << m_imageView << std::endl;
    }
}
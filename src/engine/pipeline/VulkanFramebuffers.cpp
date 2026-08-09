#include "VulkanFramebuffers.h"

VulkanFrameBuffers::VulkanFrameBuffers(
    const VkDevice& device,
    const VulkanSwapchain& swapchain,
    const VkRenderPass& renderPass) :
m_device { device }
{
    const auto& imageViews = swapchain.getImageViews();
    const auto& extent = swapchain.getSwapchainExtent();

    m_framebuffers.reserve(imageViews.size());

    for(const auto& imageView : imageViews)
    {
        VkFramebufferCreateInfo framebufferCreateInfo
        {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .renderPass = renderPass,
            .attachmentCount = 1,
            .pAttachments = &imageView.getRawImageView(),
            .width = extent.width,
            .height = extent.height,
            .layers = 1,
        };

        VkFramebuffer newFramebuffer;
        VK_THROW_IF_FAILED(vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &newFramebuffer));
        m_framebuffers.emplace_back(newFramebuffer);

        std::cout << "Vulkan framebuffer created: " << m_framebuffers.back() << std::endl;
    }
}


VulkanFrameBuffers::~VulkanFrameBuffers()
{
    for (const auto& framebuffer : m_framebuffers)
    {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);

        std::cout << "Vulkan framebuffer destroyed: " << framebuffer << std::endl;
    }
}
#include "VulkanRenderPass.h"

VulkanRenderPass::VulkanRenderPass(const VkDevice& device, const VkFormat& format) :
m_device { device }
{
    if (!m_device)
    {
        throw std::runtime_error("Render pass created without logical device");
    }

    const VkAttachmentDescription attachment
    {
        .flags = 0,
        .format = format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    // This attachment reference is used by the frag shader: layout(location = 0) out vec4 outColor
    const VkAttachmentReference attachmentReference
    {
        .attachment = 0, // Index of attachment array
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    const VkSubpassDescription subpassDescription
    {
        .flags = 0,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .inputAttachmentCount = 0,
        .pInputAttachments = nullptr,
        .colorAttachmentCount = 1,
        .pColorAttachments = &attachmentReference,
        .pResolveAttachments = nullptr,
        .pDepthStencilAttachment = nullptr,
        .preserveAttachmentCount = 0,
        .pPreserveAttachments = nullptr,
    };

    const VkSubpassDependency subpassDependency
    {
        .srcSubpass = VK_SUBPASS_EXTERNAL, // Implicit first subpass
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        .dependencyFlags = 0,
    };

    const VkRenderPassCreateInfo renderPassCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .attachmentCount = 1,
        .pAttachments = &attachment,
        .subpassCount = 1,
        .pSubpasses = &subpassDescription,
        .dependencyCount = 1,
        .pDependencies = &subpassDependency,
    };

    VK_THROW_IF_FAILED(vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_renderPass));

    LOG_ENGINE_INFO("Vulkan render pass created: " << m_renderPass);
}

VulkanRenderPass::~VulkanRenderPass()
{
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);

    LOG_ENGINE_INFO("Vulkan render pass destroyed: " << m_renderPass);
}
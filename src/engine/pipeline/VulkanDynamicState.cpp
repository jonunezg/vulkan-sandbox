#include "VulkanDynamicState.h"

VulkanDynamicState::VulkanDynamicState(const VulkanSwapchain& swapchain) :
m_swapchain { swapchain }
{
    const auto& extent = m_swapchain.getSwapchainExtent();

    m_viewport =
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    m_scissor =
    {
        .offset = { 0, 0 },
        .extent = extent,
    };

    m_dynamicStates = 
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
}

const VkPipelineDynamicStateCreateInfo VulkanDynamicState::getDynamicStateCreateInfo() const
{
    return
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .dynamicStateCount = static_cast<uint32_t>(m_dynamicStates.size()),
        .pDynamicStates = m_dynamicStates.data(),
    };
}

const VkPipelineViewportStateCreateInfo VulkanDynamicState::getViewportStateCreateInfo() const
{
    return
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .viewportCount = 1,
        .pViewports = nullptr, // Specified dynamically after setup
        .scissorCount = 1,
        .pScissors = nullptr, // Specified dynamically after setup
    };
}
#include "VulkanDynamicState.h"

VulkanDynamicState::VulkanDynamicState()
{
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
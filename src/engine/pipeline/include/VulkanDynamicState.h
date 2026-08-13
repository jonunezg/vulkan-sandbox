#pragma once

#include "Utilities.h"
#include "VulkanSwapchain.h"
#include "VulkanIncludes.h"

class VulkanDynamicState
{
public:
    VulkanDynamicState(const VulkanSwapchain& vulkanDeviceManager);

    const VkPipelineDynamicStateCreateInfo getDynamicStateCreateInfo() const;
    const VkPipelineViewportStateCreateInfo getViewportStateCreateInfo() const;

private:
    const VulkanSwapchain& m_swapchain;

    VkViewport m_viewport;
    VkRect2D m_scissor;
    std::vector<VkDynamicState> m_dynamicStates;
};
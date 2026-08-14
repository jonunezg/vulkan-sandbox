#pragma once

#include "Utilities.h"
#include "VulkanSwapchain.h"
#include "VulkanIncludes.h"

class VulkanDynamicState
{
public:
    VulkanDynamicState();

    const VkPipelineDynamicStateCreateInfo getDynamicStateCreateInfo() const;
    const VkPipelineViewportStateCreateInfo getViewportStateCreateInfo() const;

private:
    std::vector<VkDynamicState> m_dynamicStates;
};
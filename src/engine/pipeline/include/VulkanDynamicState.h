#pragma once

#include "Utilities.h"
#include "VulkanDeviceManager.h"
#include "VulkanIncludes.h"

class VulkanDynamicState
{
public:
    VulkanDynamicState(const std::shared_ptr<VulkanDeviceManager> vulkanDeviceManager);

    const VkPipelineDynamicStateCreateInfo getDynamicStateCreateInfo() const;
    const VkPipelineViewportStateCreateInfo getViewportStateCreateInfo() const;

private:
    const std::shared_ptr<VulkanDeviceManager> m_vulkanDeviceManager;

    VkViewport m_viewport;
    VkRect2D m_scissor;
    std::vector<VkDynamicState> m_dynamicStates;
};
#include "VulkanDynamicState.h"

VulkanDynamicState::VulkanDynamicState(const std::shared_ptr<VulkanDeviceManager> deviceManager) :
m_vulkanDeviceManager { std::move(deviceManager) }
{
    if (!m_vulkanDeviceManager)
    {
        throw std::runtime_error("Dynamic state created without device manager");
    }

    const auto& extent = m_vulkanDeviceManager->getSwapchainExtent();

    VkViewport viewport
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    VkRect2D scissor
    {
        .offset = { 0, 0 },
        .extent = extent,
    };
}
#pragma once

#include "VulkanDeviceManager.h"
#include "VulkanDynamicState.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderModule.h"

class VulkanPipelineManager
{
public:
    VulkanPipelineManager(const std::vector<Shader>& shaders);
    ~VulkanPipelineManager();

    VulkanPipelineManager(VulkanPipelineManager&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&) = delete;
    VulkanPipelineManager(VulkanPipelineManager&&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&&) = delete;

    bool shouldClose() { return m_vulkanDeviceManager->shouldClose(); }

private:
    const std::shared_ptr<VulkanDeviceManager> m_vulkanDeviceManager = std::make_shared<VulkanDeviceManager>();
    const VulkanDynamicState m_vulkanDynamicState{ m_vulkanDeviceManager };
    const VulkanRenderPass m_vulkanRenderPass{ m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanDeviceManager->getSwapchainFormat() };

    VkPipelineLayout m_vulkanPipelineLayout;
    std::vector<VulkanShaderModule> m_shaders;

    VkPipeline m_pipeline;
};
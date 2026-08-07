#pragma once

#include "VulkanDeviceManager.h"
#include "VulkanDynamicState.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderModule.h"

class VulkanPipelineManager
{
public:
    VulkanPipelineManager();
    ~VulkanPipelineManager();

    VulkanPipelineManager(VulkanPipelineManager&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&) = delete;
    VulkanPipelineManager(VulkanPipelineManager&&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&&) = delete;

    bool shouldClose() { return m_vulkanDeviceManager->shouldClose(); }

    void LoadShader(const std::string& path, ShaderType type)
    {
        VulkanShaderModule shader { path, type, m_vulkanDeviceManager->getLogicalDevice() };
    }

private:
    const std::shared_ptr<VulkanDeviceManager> m_vulkanDeviceManager = std::make_shared<VulkanDeviceManager>();
    const VulkanDynamicState m_vulkanDynamicState{ m_vulkanDeviceManager };
    const VulkanRenderPass m_vulkanRenderPass{ m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanDeviceManager->getSwapchainFormat() };

    VkPipelineLayout m_vulkanPipelineLayout;
};
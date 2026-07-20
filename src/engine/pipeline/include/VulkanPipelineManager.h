#pragma once

#include "VulkanDeviceManager.h"
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

    bool shouldClose() { return m_vulkanDeviceManager.shouldClose(); }

    void LoadShader(const std::string& path, ShaderType type)
    {
        VulkanShaderModule shader { path, type, m_vulkanDeviceManager.getLogicalDevice() };
    }

private:
    VulkanDeviceManager m_vulkanDeviceManager;
};
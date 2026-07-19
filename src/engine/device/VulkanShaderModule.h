#pragma once

#include "BinaryFile.h"
#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogicalDevice.h"

enum ShaderType
{
    Vertex,
    Fragment,
    Last = Fragment,
};

class VulkanShaderModule
{
public:
    VulkanShaderModule(const std::string& path, ShaderType type, const std::shared_ptr<VulkanLogicalDevice> logicalDevice);

    VulkanShaderModule(VulkanShaderModule&) = delete;
    VulkanShaderModule(const VulkanShaderModule&) = delete;
    VulkanShaderModule(VulkanShaderModule&&) = delete;
    VulkanShaderModule(const VulkanShaderModule&&) = delete;

    ~VulkanShaderModule();

    VkPipelineShaderStageCreateInfo getCreateInfo();
private:
    const BinaryFile m_file;
    const ShaderType m_type;
    const std::shared_ptr<VulkanLogicalDevice> m_logicalDevice;

    const std::string m_entryPoint = "main";

    VkShaderModule m_shaderModule;
};
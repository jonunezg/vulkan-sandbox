#pragma once

#include "BinaryFile.h"
#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogicalDevice.h"

class VulkanShaderModule
{
public:
    VulkanShaderModule(const std::string& path, const std::shared_ptr<VulkanLogicalDevice> logicalDevice);

    VulkanShaderModule(VulkanShaderModule&) = delete;
    VulkanShaderModule(const VulkanShaderModule&) = delete;
    VulkanShaderModule(VulkanShaderModule&&) = delete;
    VulkanShaderModule(const VulkanShaderModule&&) = delete;

    ~VulkanShaderModule();

private:
    const BinaryFile m_file;
    const std::shared_ptr<VulkanLogicalDevice> m_logicalDevice;

    VkShaderModule m_shaderModule;
};
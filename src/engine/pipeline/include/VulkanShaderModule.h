#pragma once

#include "BinaryFile.h"
#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"
#include "VulkanLogicalDevice.h"

enum ShaderType
{
    Vertex,
    Fragment,
    Last = Fragment,
};

typedef struct
{
    ShaderType type;
    const std::string& path;
} Shader;

class VulkanShaderModule
{
public:
    VulkanShaderModule(const Shader& definition, const VkDevice& device);

    VulkanShaderModule(VulkanShaderModule&) = delete;
    VulkanShaderModule(const VulkanShaderModule&) = delete;
    VulkanShaderModule(VulkanShaderModule&& other); // Support vector storage
    VulkanShaderModule(const VulkanShaderModule&&) = delete;

    ~VulkanShaderModule();

    VkPipelineShaderStageCreateInfo getCreateInfo();
private:
    const BinaryFile m_file;
    const ShaderType m_type;
    const VkDevice m_device;

    const std::string m_entryPoint = "main";

    VkShaderModule m_shaderModule;
};
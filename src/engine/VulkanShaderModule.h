#pragma once

#include "BinaryFile.h"
#include "Utilities.h"
#include "VulkanIncludes.h"

class VulkanShaderModule
{
public:
    VulkanShaderModule(const std::string& path);

    VulkanShaderModule(VulkanShaderModule&) = delete;
    VulkanShaderModule(const VulkanShaderModule&) = delete;
    VulkanShaderModule(VulkanShaderModule&) = delete;
    VulkanShaderModule(const VulkanShaderModule&&) = delete;

    ~VulkanShaderModule();

private:
    const BinaryFile m_file;
};
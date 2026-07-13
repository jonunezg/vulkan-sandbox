#include "VulkanShaderModule.h"

VulkanShaderModule::VulkanShaderModule(
    const std::string& path,
    const std::shared_ptr<VulkanLogicalDevice> logicalDevice) :
m_file { path },
m_logicalDevice { std::move(logicalDevice) }
{
    if (!m_logicalDevice)
    {
        throw std::runtime_error("Shader module created without logical device");
    }

    const VkShaderModuleCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .codeSize = m_file.getData().size(),
        .pCode = reinterpret_cast<const uint32_t*>(m_file.getData().data()),
    };

    VK_THROW_IF_FAILED(vkCreateShaderModule(m_logicalDevice->getDevice(), &createInfo, nullptr, &m_shaderModule));

    std::cout << "Loaded shader: " << path << ", size: " << m_file.getData().size() << std::endl;
}

VulkanShaderModule::~VulkanShaderModule()
{
    vkDestroyShaderModule(m_logicalDevice->getDevice(), m_shaderModule, nullptr);
}
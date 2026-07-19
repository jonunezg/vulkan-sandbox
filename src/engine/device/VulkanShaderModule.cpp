#include "VulkanShaderModule.h"

VulkanShaderModule::VulkanShaderModule(
    const std::string& path,
    ShaderType type,
    const std::shared_ptr<VulkanLogicalDevice> logicalDevice) :
m_file { path },
m_type { type },
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
    std::cout << "Unloaded shader" << std::endl;
}

VkPipelineShaderStageCreateInfo VulkanShaderModule::getCreateInfo()
{
    VkShaderStageFlagBits stage;
    
    static_assert(Fragment == Last, "New type of shader needs Vulkan shader stage bit");

    switch (m_type)
    {
    case ShaderType::Vertex:
        stage = VK_SHADER_STAGE_VERTEX_BIT;
        break;
    case ShaderType::Fragment:
        stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        break;
    default:
        throw std::runtime_error("New shader type lacks Vulkan shader stage bit, value: " + m_type);
    }

    return
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = stage,
        .module = m_shaderModule,
        .pName = m_entryPoint.c_str(),
        .pSpecializationInfo = nullptr,
    };
}
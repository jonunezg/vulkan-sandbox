#include "VulkanShaderModule.h"

VulkanShaderModule::VulkanShaderModule(
    const Shader& definition,
    const VkDevice& device) :
m_file { definition.path },
m_type { definition.type },
m_device { std::move(device) }
{
    if (!m_device)
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

    VK_THROW_IF_FAILED(vkCreateShaderModule(m_device, &createInfo, nullptr, &m_shaderModule));

    std::cout << "Loaded shader: " << definition.path << ", size: " << m_file.getData().size() << ", address: " << m_shaderModule << std::endl;
}

VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& other) :
m_file { other.m_file },
m_type { other.m_type },
m_device { std::move(other.m_device) },
m_entryPoint { std::move(other.m_entryPoint) },
m_shaderModule { other.m_shaderModule }
{
    other.m_shaderModule = nullptr;
}

VulkanShaderModule::~VulkanShaderModule()
{
    if (m_shaderModule)
    {
        vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
        std::cout << "Unloaded shader from address: " << m_shaderModule << std::endl;
    }
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
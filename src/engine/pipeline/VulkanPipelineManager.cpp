#include "VulkanPipelineManager.h"

VulkanPipelineManager::VulkanPipelineManager(const std::vector<Shader>& shaders)
{
    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = 0,
        .pVertexBindingDescriptions = nullptr,
        .vertexAttributeDescriptionCount = 0,
        .pVertexAttributeDescriptions = nullptr,
    };

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment
    {
        // blendEnable ? final = use struct operators : final = new
        .blendEnable = VK_FALSE,
        // final.rgb = (new.rgb * srcColorBlendFactor) colorBlendOp (old.rgb * dstColorBlendFactor)
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        // final.a = (new.a * srcAlphaBlendFactor) alphaBlendOp (old.a * dstAlphaBlendFactor)
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        // final &= colorWriteMask. This step is always applied
        .colorWriteMask = VK_COLOR_COMPONENT_R_BIT
                        | VK_COLOR_COMPONENT_G_BIT
                        | VK_COLOR_COMPONENT_B_BIT
                        | VK_COLOR_COMPONENT_A_BIT,
    };

    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &colorBlendAttachment,
        .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
    };

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = nullptr,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    VK_THROW_IF_FAILED(vkCreatePipelineLayout(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), &pipelineLayoutCreateInfo, nullptr, &m_vulkanPipelineLayout));

    std::cout << "Vulkan pipeline layout created" << std::endl;

    std::vector<VkPipelineShaderStageCreateInfo> shadersCreateInfo {};
    shadersCreateInfo.reserve(shaders.size());
    m_shaders.reserve(shaders.size());
    for(const auto& shader : shaders)
    {
        m_shaders.emplace_back(shader, m_vulkanDeviceManager->getLogicalDevice()->getDevice());
        shadersCreateInfo.emplace_back(m_shaders.back().getCreateInfo());
    }

    const auto viewportStateCreateInfo = m_vulkanDynamicState.getViewportStateCreateInfo();
    const auto dynamicStateCreateInfo = m_vulkanDynamicState.getDynamicStateCreateInfo();

    VkGraphicsPipelineCreateInfo pipelineCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = nullptr,
        .stageCount = 2,
        .pStages = shadersCreateInfo.data(),
        .pVertexInputState = &vertexInputCreateInfo,
        .pInputAssemblyState = &inputAssemblyCreateInfo,
        .pTessellationState = nullptr,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        .pDepthStencilState = nullptr,
        .pColorBlendState = &colorBlendStateCreateInfo,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = m_vulkanPipelineLayout,
        .renderPass = m_vulkanRenderPass.getRenderPass(),
        .subpass = 0,
        .basePipelineHandle = VK_NULL_HANDLE,
        .basePipelineIndex = -1,
    };

    VK_THROW_IF_FAILED(vkCreateGraphicsPipelines(
        m_vulkanDeviceManager->getLogicalDevice()->getDevice(),
        VK_NULL_HANDLE,
        1,
        &pipelineCreateInfo,
        nullptr,
        &m_pipeline));

    std::cout << "Vulkan graphics pipeline created" << std::endl;
}

VulkanPipelineManager::~VulkanPipelineManager()
{
    vkDestroyPipeline(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_pipeline, nullptr);
    std::cout << "Vulkan graphics pipeline destroyed" << std::endl;

    vkDestroyPipelineLayout(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanPipelineLayout, nullptr);
    std::cout << "Vulkan pipeline layout destroyed" << std::endl;
}
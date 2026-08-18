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

    LOG_ENGINE_INFO("Vulkan pipeline layout created: " << m_vulkanPipelineLayout);

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
        .renderPass = m_vulkanRenderPass->getRenderPass(),
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

    LOG_ENGINE_INFO("Vulkan graphics pipeline created: " << m_pipeline);

    VkSemaphoreCreateInfo semaphoreCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
    };

    VkFenceCreateInfo fenceCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT,
    };

    
    m_imageAvailableSemaphores.resize(MAX_CONCURRENT_IMAGES);
    m_inFlightFences.resize(MAX_CONCURRENT_IMAGES);
    for (size_t i = 0 ; i < MAX_CONCURRENT_IMAGES ; i++)
    {
        VK_THROW_IF_FAILED(vkCreateSemaphore(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphores[i]));
        VK_THROW_IF_FAILED(vkCreateFence(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), &fenceCreateInfo, nullptr, &m_inFlightFences[i]));
    }

    const auto imageCount = m_swapchain->getImageViews().size();
    m_renderFinishedSemaphores.resize(imageCount);
    for (size_t i = 0 ; i < imageCount ; i++)
    {
        VK_THROW_IF_FAILED(vkCreateSemaphore(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphores[i]));
    }

}

VulkanPipelineManager::~VulkanPipelineManager()
{
    vkDestroyPipeline(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_pipeline, nullptr);
    LOG_ENGINE_INFO("Vulkan graphics pipeline destroyed: " << m_pipeline);

    vkDestroyPipelineLayout(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanPipelineLayout, nullptr);
    LOG_ENGINE_INFO("Vulkan pipeline layout destroyed: " << m_vulkanPipelineLayout);

    
    for (size_t i = 0 ; i < MAX_CONCURRENT_IMAGES ; i++)
    {
        vkDestroySemaphore(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_inFlightFences[i], nullptr);
    }

    for (size_t i = 0 ; i < m_renderFinishedSemaphores.size() ; i++)
    {
        vkDestroySemaphore(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_renderFinishedSemaphores[i], nullptr);
    }

}

bool VulkanPipelineManager::drawFrame()
{
    const VkDevice& device = m_vulkanDeviceManager->getLogicalDevice()->getDevice();
    const VkSwapchainKHR& swapchain = m_swapchain->getSwapchain();
    const VkCommandBuffer& commandBuffer = m_vulkanCommandPool.getCommandBuffer(m_frameIndex);
    const VkQueue& graphicsQueue = m_vulkanDeviceManager->getLogicalDevice()->getGraphicsQueue();
    const VkQueue& presentQueue = m_vulkanDeviceManager->getLogicalDevice()->getPresentQueue();

    if (shouldClose())
    {
        waitDeviceIdle();
        return false;
    }

    vkWaitForFences(device, 1, &m_inFlightFences[m_frameIndex], VK_TRUE, UINT64_MAX);
    
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, m_imageAvailableSemaphores[m_frameIndex], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        recreateSwapchainObjects();
        return true;
    }
    VK_THROW_IF_FAILED(result);

    vkResetFences(device, 1, &m_inFlightFences[m_frameIndex]);

    vkResetCommandBuffer(commandBuffer, 0);

    m_vulkanCommandPool.recordCommandBuffer(
        m_frameIndex,
        imageIndex,
        m_vulkanRenderPass->getRenderPass(),
        m_vulkanFramebuffers->getFramebuffers(),
        m_swapchain->getSwapchainExtent(),
        m_pipeline);

    const VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    const VkSubmitInfo submitInfo
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_imageAvailableSemaphores[m_frameIndex],
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &commandBuffer,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &m_renderFinishedSemaphores[imageIndex],
    };

    VK_THROW_IF_FAILED(vkQueueSubmit(graphicsQueue, 1, &submitInfo, m_inFlightFences[m_frameIndex]));

    const VkPresentInfoKHR presentInfo
    {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &m_renderFinishedSemaphores[imageIndex],
        .swapchainCount = 1,
        .pSwapchains = &swapchain,
        .pImageIndices = &imageIndex,
        .pResults = nullptr,
    };

    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_vulkanDeviceManager->getWindowManager().windowResized())
    {
        recreateSwapchainObjects();
    }
    else
    {
        VK_THROW_IF_FAILED(result);
    }

    m_frameIndex = (m_frameIndex + 1) % MAX_CONCURRENT_IMAGES;

    return true;
}

void VulkanPipelineManager::waitDeviceIdle()
{
    vkDeviceWaitIdle(m_vulkanDeviceManager->getLogicalDevice()->getDevice());
}

void VulkanPipelineManager::recreateSwapchainObjects()
{
    LOG_ENGINE_WARNING("Swapchain objects recreation started");

    waitDeviceIdle();
    m_vulkanDeviceManager->getWindowManager().pauseWhileMinimized();
    m_swapchain.reset();
    m_vulkanRenderPass.reset();
    m_vulkanFramebuffers.reset();
    m_swapchain = std::make_unique<VulkanSwapchain>(m_vulkanDeviceManager->getSharedPhysicalDevice(), m_vulkanDeviceManager->getLogicalDevice(), m_vulkanDeviceManager->getSharedSurface(), m_vulkanDeviceManager->getWindowManager());
    m_vulkanRenderPass = std::make_unique<VulkanRenderPass>(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_swapchain->getSwapchainFormat());
    m_vulkanFramebuffers = std::make_unique<VulkanFrameBuffers>(m_vulkanDeviceManager->getLogicalDevice()->getDevice(), *m_swapchain, m_vulkanRenderPass->getRenderPass());

    LOG_ENGINE_WARNING("Swapchain objects recreation complete");
}
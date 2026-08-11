#pragma once

#include "VulkanCommandPool.h"
#include "VulkanDeviceManager.h"
#include "VulkanDynamicState.h"
#include "VulkanFramebuffers.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderModule.h"

class VulkanPipelineManager
{
public:
    VulkanPipelineManager(const std::vector<Shader>& shaders);
    ~VulkanPipelineManager();

    VulkanPipelineManager(VulkanPipelineManager&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&) = delete;
    VulkanPipelineManager(VulkanPipelineManager&&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&&) = delete;

    bool shouldClose() { return m_vulkanDeviceManager->shouldClose(); }

    void drawFrame();

    void waitDeviceIdle();

private:
    const std::shared_ptr<VulkanDeviceManager> m_vulkanDeviceManager = std::make_shared<VulkanDeviceManager>();
    const VulkanDynamicState m_vulkanDynamicState{ m_vulkanDeviceManager };
    const VulkanRenderPass m_vulkanRenderPass{ m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanDeviceManager->getSwapchainFormat() };
    const VulkanFrameBuffers m_vulkanFramebuffers{ m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanDeviceManager->getSwapchain(), m_vulkanRenderPass.getRenderPass() };
    
    VulkanCommandPool m_vulkanCommandPool{ m_vulkanDeviceManager->getLogicalDevice()->getDevice(), m_vulkanDeviceManager->getPhysicalDevice() };

    VkPipelineLayout m_vulkanPipelineLayout;
    std::vector<VulkanShaderModule> m_shaders;

    VkPipeline m_pipeline;

    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    VkFence m_inFlightFence;
};
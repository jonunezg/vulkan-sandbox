#pragma once

#include "VulkanVertexBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanDeviceManager.h"
#include "VulkanDynamicState.h"
#include "VulkanFramebuffers.h"
#include "VulkanRenderPass.h"
#include "VulkanShaderModule.h"
#include "VulkanVertexCommandBuffers.h"
#include "VulkanVertexInput.h"

class VulkanPipelineManager
{
public:
    VulkanPipelineManager(const std::vector<Shader>& shaders, const std::vector<Geometry::Vertex>& vertices);
    ~VulkanPipelineManager();

    VulkanPipelineManager(VulkanPipelineManager&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&) = delete;
    VulkanPipelineManager(VulkanPipelineManager&&) = delete;
    VulkanPipelineManager(const VulkanPipelineManager&&) = delete;
    
    bool drawFrame();

private:
    void recreateSwapchainObjects();
    bool shouldClose() { return m_vulkanDeviceManager.shouldClose(); }
    void waitDeviceIdle();

    VulkanDeviceManager m_vulkanDeviceManager {};
    const VulkanDynamicState m_vulkanDynamicState {};

    std::unique_ptr<VulkanSwapchain> m_swapchain = std::make_unique<VulkanSwapchain>(m_vulkanDeviceManager.getPhysicalDevice(), m_vulkanDeviceManager.getLogicalDevice().getDevice(), m_vulkanDeviceManager.getSurface(), m_vulkanDeviceManager.getWindowManager());
    std::unique_ptr<VulkanRenderPass> m_vulkanRenderPass = std::make_unique<VulkanRenderPass>(m_vulkanDeviceManager.getLogicalDevice().getDevice(), m_swapchain->getSwapchainFormat());
    std::unique_ptr<VulkanFrameBuffers> m_vulkanFramebuffers = std::make_unique<VulkanFrameBuffers>(m_vulkanDeviceManager.getLogicalDevice().getDevice(), *m_swapchain, m_vulkanRenderPass->getRenderPass());
    
    VulkanCommandPool m_vulkanCommandPool{ m_vulkanDeviceManager.getLogicalDevice().getDevice(), m_vulkanDeviceManager.getPhysicalDevice() };

    VulkanVertexBuffer m_vertexBuffer;
    VulkanVertexCommandBuffers m_vertexCommandBuffers;

    VkPipelineLayout m_vulkanPipelineLayout;
    std::vector<VulkanShaderModule> m_shaders;

    VkPipeline m_pipeline;

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinishedSemaphores;
    std::vector<VkFence> m_inFlightFences;

    size_t m_frameIndex = 0;
};
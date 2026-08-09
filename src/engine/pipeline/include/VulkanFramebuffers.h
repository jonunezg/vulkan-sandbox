#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanSwapchain.h"

class VulkanFrameBuffers
{
public:
    VulkanFrameBuffers(const VkDevice& device, const VulkanSwapchain& swapchain, const VkRenderPass& renderPass);
    ~VulkanFrameBuffers();

    VulkanFrameBuffers(VulkanFrameBuffers&) = delete;
    VulkanFrameBuffers(const VulkanFrameBuffers&) = delete;
    VulkanFrameBuffers(VulkanFrameBuffers&&) = delete;
    VulkanFrameBuffers(const VulkanFrameBuffers&&) = delete;

private:
    const VkDevice m_device;
    std::vector<VkFramebuffer> m_framebuffers;
};
#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"

class VulkanRenderPass
{
public:
    VulkanRenderPass(const VkDevice& device, const VkFormat& format);

    ~VulkanRenderPass();

    const VkRenderPass& getRenderPass() const { return m_renderPass; }

private:
    const VkDevice& m_device;
    VkRenderPass m_renderPass;
};
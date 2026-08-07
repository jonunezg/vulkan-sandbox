#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"

class VulkanRenderPass
{
public:
    VulkanRenderPass(const VkDevice& device, const VkFormat& format);

    ~VulkanRenderPass();

private:
    const VkDevice& m_device;
    VkRenderPass m_renderPass;
};
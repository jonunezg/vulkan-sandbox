#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"

class VulkanRenderPass
{
public:
    VulkanRenderPass(const VkFormat& format);

    ~VulkanRenderPass();
};
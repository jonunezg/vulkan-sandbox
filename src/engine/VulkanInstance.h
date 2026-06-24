#pragma once

#include "VulkanExtensions.h"
#include "VulkanIncludes.h"

class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();

    const VkInstance getInstance() { return m_instance; }
private:
    VkInstance m_instance;
};
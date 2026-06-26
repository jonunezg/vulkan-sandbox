#pragma once


#include "VulkanIncludes.h"

class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();

    const VkInstance getInstance() const { return m_instance; }
private:
    VkInstance m_instance;
};
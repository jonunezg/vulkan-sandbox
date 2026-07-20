#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"

class VulkanLogger
{
public:
    VulkanLogger(std::shared_ptr<VulkanInstance> instance);

    static const VkDebugUtilsMessengerCreateInfoEXT *getDebuggerMessengerInfo();

    ~VulkanLogger();

private:
    VkDebugUtilsMessengerEXT m_debugMessenger;
    const std::shared_ptr<VulkanInstance> m_instance;
};
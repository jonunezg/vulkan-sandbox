#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"

class VulkanInstance;

class VulkanLogger
{
public:
    VulkanLogger(std::shared_ptr<VulkanInstance> instance);

    static const VkDebugUtilsMessengerCreateInfoEXT *getDebuggerMessengerInfo();

    static std::vector<const char*> getValidationLayers();

    ~VulkanLogger();

private:
    VkDebugUtilsMessengerEXT m_debugMessenger;
    const std::shared_ptr<VulkanInstance> m_instance;
};
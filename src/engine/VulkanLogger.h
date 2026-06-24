#pragma once

#include <vector>

#include "VulkanIncludes.h"

class VulkanLogger
{
public:
    VulkanLogger(VkInstance instance);

    static const VkDebugUtilsMessengerCreateInfoEXT *getDebuggerMessengerInfo();

    static std::vector<const char*> getValidationLayers();

    ~VulkanLogger();

private:
    VkDebugUtilsMessengerEXT m_debugMessenger;
    // TODO: Wrap instance and probably do shared_ptr to ensure lifetime
    VkInstance m_instance;
};
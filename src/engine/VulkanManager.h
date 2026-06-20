#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "Utilities.h"

class VulkanManager
{
public:
    VulkanManager();
    ~VulkanManager();

    VulkanManager(VulkanManager&) = delete;
    VulkanManager(const VulkanManager&) = delete;
    VulkanManager(VulkanManager&&) = delete;
    VulkanManager(const VulkanManager&&) = delete;

    void initVulkan();

private:
    void initDebugMessenger();
    void createInstance();
    std::vector<const char*> getExtensions();
    std::vector<const char*> getValidationLayers();

    VkInstance m_instance;
    const bool m_enableVulkanDebug
#ifndef NDEBUG
        = true;
#else
        = false;
#endif
    VkDebugUtilsMessengerEXT m_debugMessenger;

};
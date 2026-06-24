#pragma once

#include "VulkanExtensions.h"
#include "VulkanLogger.h"
#include "VulkanIncludes.h"

class VulkanInstance
{
public:
    VulkanInstance()
    {
        const VkApplicationInfo appInfo
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "Hello triangle",
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName = "Quetzalcoatl",
            .engineVersion = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion = VK_API_VERSION_1_0,
        };

        const auto extensions = getExtensions();
        const auto layers = VulkanLogger::getValidationLayers();
        const auto debugger = VulkanLogger::getDebuggerMessengerInfo();
        
        const VkInstanceCreateInfo createInfo
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = debugger,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = static_cast<uint32_t>(layers.size()),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        VK_TERMINATE_IF_FAILED(vkCreateInstance(&createInfo, nullptr, &m_instance));

        std::cout << "Vulkan instance created" << std::endl;
    }

    ~VulkanInstance()
    {}

    const VkInstance getInstance() { return m_instance; }
private:
    VkInstance m_instance;
};
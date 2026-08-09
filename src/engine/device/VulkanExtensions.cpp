#include "Utilities.h"
#include "VulkanExtensions.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"

std::vector<VkExtensionProperties> getAvailableExtensions()
{
    uint32_t count = 0;
    VK_THROW_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));

    std::vector<VkExtensionProperties> extensions(count);

    VK_THROW_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data()));

    return extensions;
}

void dumpAvailableExtensions()
{
    const auto extensions = getAvailableExtensions();

    LOG_ENGINE_VERBOSE("Available Vulkan extensions:"s);
    for(const auto& extension : extensions)
    {
        LOG_ENGINE_VERBOSE("\t"s + extension.extensionName + ":" + std::to_string(extension.specVersion));
    }
}

void dumpRequiredExtensions(const std::vector<const char*>& extensions)
{
    LOG_ENGINE_INFO("Required "s + std::to_string(extensions.size()) + " Vulkan extensions:");

    for (const auto& extension : extensions)
    {
        LOG_ENGINE_INFO("\t"s + extension);
    }
}

void addDebugExtensions(std::vector<const char *>& extensions)
{
    if (VK_ENABLE_DEBUG)
    {
        extensions.push_back(VK_REQUIRED_VALIDATION_EXTENSION);
        dumpAvailableExtensions();
        dumpRequiredExtensions(extensions);
    }
}

std::vector<const char *> getExtensions()
{
    uint32_t glfwExtensionsCount;
    const auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

    if (!glfwExtensions)
    {
        throw std::runtime_error("GLFW returned empty list of Vulkan extensions");
    }

    std::vector<const char *> extensions = { glfwExtensions, glfwExtensions + glfwExtensionsCount };

    addDebugExtensions(extensions);

    return extensions;
}

const std::vector<const char *> getRequiredPhysicalDeviceExtensions()
{
    return std::vector<const char *>{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
}
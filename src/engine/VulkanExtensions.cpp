
#include "Utilities.h"
#include "VulkanExtensions.h"
#include "VulkanIncludes.h"

std::vector<VkExtensionProperties> getAvailableExtensions()
{
    uint32_t count = 0;
    VK_TERMINATE_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));

    std::vector<VkExtensionProperties> extensions(count);

    VK_TERMINATE_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data()));

    return extensions;
}

void dumpAvailableExtensions()
{
    const auto extensions = getAvailableExtensions();

    std::cout << "Available Vulkan extensions:" << std::endl;
    for(const auto& extension : extensions)
    {
        std::cout << "\t" << extension.extensionName << ":" << extension.specVersion << std::endl;
    }
}

void dumpRequiredExtensions(const std::vector<const char*>& extensions)
{
    std::cout << "Required " << extensions.size() << " Vulkan extensions:" << std::endl;

    for (const auto& extension : extensions)
    {
        std::cout << "\t" << extension << std::endl;
    }
}

void addDebugExtensions(std::vector<const char *>& extensions)
{
    if (m_enableVulkanDebug)
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
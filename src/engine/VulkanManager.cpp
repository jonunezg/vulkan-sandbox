
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "VulkanManager.h"

// Vulkan debug callback

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* /* userData */) {

    std::string typeString;
    switch (type)
    {
    case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        typeString = GREEN "[General]" RESET;
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        typeString = CYAN "[Validation]" RESET;
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        typeString = ORANGE "[Performance]" RESET;
        break;
    case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
        typeString = PEACH "[DeviceAddressBinding]" RESET;
        break;
    default:
        typeString = "[Unknown]";
    }

    std::string severityString;
    switch (severity)
    {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        severityString = PURPLE "[Verbose]" RESET;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        severityString = BLUE "[Info]" RESET;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        severityString = YELLOW "[Warning]" RESET;
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        severityString = RED "[Error]" RESET;
        break;
    default:
        severityString = "[Unknown]";
    }

    std::cout << severityString << typeString << ": " << data->pMessage << std::endl;

    return VK_FALSE;
}

// Calls to instance functions

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* createInfo, VkDebugUtilsMessengerEXT* messenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, createInfo, nullptr, messenger);
    } 
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, messenger, nullptr);
    }
}

// Local helpers

const VkDebugUtilsMessengerCreateInfoEXT getDebuggerMessengerInfo()
{
    return VkDebugUtilsMessengerCreateInfoEXT
    {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .flags = 0,
        .messageSeverity = 
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType =  
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallback,
        .pUserData = nullptr,
    };
}

void dumpRequiredExtensions(const std::vector<const char*>& extensions)
{
    std::cout << "Required " << extensions.size() << " Vulkan extensions:" << std::endl;

    for (const auto& extension : extensions)
    {
        std::cout << "\t" << extension << std::endl;
    }
}

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

std::vector<VkLayerProperties> getAvailableValidationLayers()
{
    uint32_t count = 0;
    VK_TERMINATE_IF_FAILED(vkEnumerateInstanceLayerProperties(&count, nullptr));

    std::vector<VkLayerProperties> properties(count);
    VK_TERMINATE_IF_FAILED(vkEnumerateInstanceLayerProperties(&count, properties.data()));

    return properties;
}

void dumpAvailableValidationLayers()
{
    const auto layers = getAvailableValidationLayers();
    std::cout << "Available Vulkan validation layers:" << std::endl;
    for(const auto& layer : layers)
    {
        std::cout << "\t" << layer.layerName << ": " << layer.description << std::endl;
    }
}

void dumpRequiredValidationLayers(const std::vector<const char*>& layers)
{
    std::cout << "Required " << layers.size() << " Vulkan validation layers:" << std::endl;
    for (const auto& layer : layers)
    {
        std::cout << "\t" << layer << std::endl;
    }
}


// Class definitions

void VulkanManager::initDebugMessenger()
{
    if (!m_enableVulkanDebug)
    {
        return;
    }

    const auto createInfo = getDebuggerMessengerInfo();

    VK_TERMINATE_IF_FAILED(createDebugUtilsMessengerEXT(m_instance, &createInfo, &m_debugMessenger));
}

VulkanManager::VulkanManager()
{}

VulkanManager::~VulkanManager()
{
    if (m_enableVulkanDebug)
    {
        destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger);
    }

    vkDestroyInstance(m_instance, nullptr);
}

void VulkanManager::initVulkan()
{
    createInstance();
    initDebugMessenger();
}

void VulkanManager::createInstance()
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
    const auto layers = getValidationLayers();
    const auto debugger = getDebuggerMessengerInfo();
    
    const VkInstanceCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = m_enableVulkanDebug ? static_cast<const VkDebugUtilsMessengerCreateInfoEXT *>(&debugger) : nullptr,
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

std::vector<const char*> VulkanManager::getExtensions()
{
    uint32_t glfwExtensionsCount;
    const auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    assert(glfwExtensions);

    std::vector<const char*> extensions = { glfwExtensions, glfwExtensions + glfwExtensionsCount };

    if (m_enableVulkanDebug)
    {
        extensions.push_back(VK_REQUIRED_VALIDATION_EXTENSION);
        dumpAvailableExtensions();
        dumpRequiredExtensions(extensions);
    }


    return extensions;
}

std::vector<const char*> VulkanManager::getValidationLayers()
{
    if (!m_enableVulkanDebug)
    {
        return {};
    }

    std::vector<const char*> layers = VK_REQUIRED_VALIDATION_LAYERS;
    dumpAvailableValidationLayers();
    dumpRequiredValidationLayers(layers);

    return layers;
}
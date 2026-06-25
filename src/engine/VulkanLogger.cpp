#include "VulkanLogger.h"

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

const VkDebugUtilsMessengerCreateInfoEXT debuggerInfo =
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

const VkDebugUtilsMessengerCreateInfoEXT *VulkanLogger::getDebuggerMessengerInfo()
{
    return VK_ENABLE_DEBUG ? &debuggerInfo : nullptr;
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

// Class implementation

VulkanLogger::VulkanLogger(std::shared_ptr<VulkanInstance> instance) :
m_instance { std::move(instance) }
{
    if (!m_instance)
    {
        throw std::runtime_error("Logger created without Vulkan instance");
    }

    if (!VK_ENABLE_DEBUG)
    {
        return;
    }

    const auto createInfo = getDebuggerMessengerInfo();

    VK_TERMINATE_IF_FAILED(createDebugUtilsMessengerEXT(m_instance->getInstance(), createInfo, &m_debugMessenger));
}

VulkanLogger::~VulkanLogger()
{
    if (m_debugMessenger)
    {
        destroyDebugUtilsMessengerEXT(m_instance->getInstance(), m_debugMessenger);
    }
}
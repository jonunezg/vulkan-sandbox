#pragma once

#include <string>
#include <sstream>
#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"

typedef enum DebugMessageType
{
    DEBUG_MESSAGE_ENGINE = 0x80000000,
    DEBUG_MESSAGE_APPLICATION = 0x40000000
} DebugMessageType;

void logDebugMessage(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    uint64_t type,
    const char* message
);

using namespace std::literals::string_literals;

#ifndef NDEBUG
    #define LOG_ENGINE_VERBOSE(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT, DEBUG_MESSAGE_ENGINE, ss.str().c_str()); }
    #define LOG_ENGINE_INFO(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, DEBUG_MESSAGE_ENGINE, ss.str().c_str()); }
    #define LOG_ENGINE_WARNING(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT, DEBUG_MESSAGE_ENGINE, ss.str().c_str()); }
    #define LOG_ENGINE_ERROR(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, DEBUG_MESSAGE_ENGINE, ss.str().c_str()); }

    #define LOG_APP_VERBOSE(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT, DEBUG_MESSAGE_APPLICATION, ss.str().c_str()); }
    #define LOG_APP_INFO(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT, DEBUG_MESSAGE_APPLICATION, ss.str().c_str()); }
    #define LOG_APP_WARNING(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT, DEBUG_MESSAGE_APPLICATION, ss.str().c_str()); }
    #define LOG_APP_ERROR(message) { std::stringstream ss; ss << message; logDebugMessage(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT, DEBUG_MESSAGE_APPLICATION, ss.str().c_str()); }
#else
    #define LOG_ENGINE_VERBOSE(message)
    #define LOG_ENGINE_INFO(message)
    #define LOG_ENGINE_WARNING(message)
    #define LOG_ENGINE_ERROR(message)

    #define LOG_APP_VERBOSE(message)
    #define LOG_APP_INFO(message)
    #define LOG_APP_WARNING(message)
    #define LOG_APP_ERROR(message)
#endif

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
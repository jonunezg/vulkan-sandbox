#pragma once

#include <iostream>
#include <memory>
#include <string>

const uint32_t WINDOW_WIDTH = 600;
const uint32_t WINDOW_HEIGHT = 800;

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[38;2;160;32;240m"
#define CYAN "\033[38;2;0;255;255m"
#define ORANGE "\033[38;2;255;165;0m"
#define PEACH "\033[38;2;255;229;180m"

#define VK_REQUIRED_VALIDATION_LAYERS { "VK_LAYER_KHRONOS_validation" }
#define VK_REQUIRED_VALIDATION_EXTENSION VK_EXT_DEBUG_UTILS_EXTENSION_NAME

#define VK_TERMINATE_IF_FAILED(x) \
do \
{ \
    VkResult result = (x); \
    if (result != VK_SUCCESS) \
    { \
        std::string msg = "!!!Vulkan error at: " __FILE__ ":" + std::to_string(__LINE__) + ", code: " + std::to_string(result); \
        throw std::runtime_error(msg.c_str()); \
    } \
} while(0);

#ifndef NDEBUG
       const bool m_enableVulkanDebug = true;
#else
       const bool m_enableVulkanDebug = false;
#endif

#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "WindowManager.h"

class VulkanSurface
{
public:
    VulkanSurface(
        GLFWwindow* window,
        const VkInstance& instance);
    ~VulkanSurface();

    const VkSurfaceKHR& getSurface() const { return m_surface; }

private:
    const VkInstance& m_instance;
    VkSurfaceKHR m_surface;
};
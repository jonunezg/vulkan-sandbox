#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "WindowManager.h"

class VulkanSurface
{
public:
    VulkanSurface(const WindowManager& windowManager,
    const VulkanInstance& instance);
    ~VulkanSurface();

    const VkSurfaceKHR getSurface() { return m_surface; }

private:
    const VulkanInstance& m_instance;
    VkSurfaceKHR m_surface;
};
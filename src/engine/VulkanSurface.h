#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "WindowManager.h"

class VulkanSurface
{
public:
    VulkanSurface(const std::shared_ptr<WindowManager> windowManager,
    const std::shared_ptr<VulkanInstance> instance);
    ~VulkanSurface();

    const VkSurfaceKHR getSurface() { return m_surface; }

private:
    const std::shared_ptr<WindowManager> m_windowManager;
    const std::shared_ptr<VulkanInstance> m_instance;
    VkSurfaceKHR m_surface;
};
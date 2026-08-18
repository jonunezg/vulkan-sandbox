#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "VulkanLogger.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSurface.h"
#include "WindowManager.h"

class VulkanDeviceManager
{
public:
    VulkanDeviceManager();
    ~VulkanDeviceManager();

    VulkanDeviceManager(VulkanDeviceManager&) = delete;
    VulkanDeviceManager(const VulkanDeviceManager&) = delete;
    VulkanDeviceManager(VulkanDeviceManager&&) = delete;
    VulkanDeviceManager(const VulkanDeviceManager&&) = delete;

    bool shouldClose() const { return m_windowManager.windowShouldClose(); }
    const VulkanLogicalDevice& getLogicalDevice() const { return m_logicalDevice; }
    const PhysicalDevice& getPhysicalDevice() { return m_physicalDevice.getSelectedDevice(); }
    const VkSurfaceKHR& getSurface() const { return m_surface.getSurface(); }
    WindowManager& getWindowManager() { return m_windowManager; }

private:
    WindowManager m_windowManager {}; // Non-const because mutates when window resizes
    const VulkanInstance m_instance {};
    const VulkanLogger m_logger = { m_instance.getInstance() };
    const VulkanSurface m_surface { m_windowManager.getWindowHandle(), m_instance.getInstance() };
    VulkanPhysicalDevice m_physicalDevice = { m_instance, m_surface }; // Non-const because selected device properties change over time
    const VulkanLogicalDevice m_logicalDevice = { m_physicalDevice.getSelectedDevice() };
};
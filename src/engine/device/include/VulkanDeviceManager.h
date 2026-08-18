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
    const std::shared_ptr<VulkanLogicalDevice> getLogicalDevice() const { return m_logicalDevice; }
    const PhysicalDevice& getPhysicalDevice() { return m_physicalDevice.getSelectedDevice(); }
    const VkSurfaceKHR& getSurface() const { return m_surface.getSurface(); }
    WindowManager& getWindowManager() { return m_windowManager; }

private:
    WindowManager m_windowManager {};
    const VulkanInstance m_instance {};
    const VulkanLogger m_logger = { m_instance };
    const VulkanSurface m_surface { m_windowManager, m_instance };
    VulkanPhysicalDevice m_physicalDevice = { m_instance, m_surface };
    const std::shared_ptr<VulkanLogicalDevice> m_logicalDevice = std::make_shared<VulkanLogicalDevice>(m_physicalDevice.getSelectedDevice());
};
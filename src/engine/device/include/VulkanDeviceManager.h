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
    const PhysicalDevice& getPhysicalDevice() const { return m_physicalDevice->getSelectedDevice(); }

    const std::shared_ptr<VulkanPhysicalDevice> getSharedPhysicalDevice() { return m_physicalDevice; }
    const std::shared_ptr<VulkanSurface> getSharedSurface() { return m_surface; }
    WindowManager& getWindowManager() { return m_windowManager; }

private:
    // Vulkan objects following RAII pattern
    WindowManager m_windowManager {};
    const VulkanInstance m_instance {};
    const VulkanLogger m_logger = { m_instance };
    const std::shared_ptr<VulkanSurface> m_surface = std::make_shared<VulkanSurface>(m_windowManager, m_instance);
    const std::shared_ptr<VulkanPhysicalDevice> m_physicalDevice = std::make_shared<VulkanPhysicalDevice>(m_instance, m_surface);
    const std::shared_ptr<VulkanLogicalDevice> m_logicalDevice = std::make_shared<VulkanLogicalDevice>(m_physicalDevice);
};
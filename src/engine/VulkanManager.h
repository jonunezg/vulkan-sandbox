#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "VulkanLogger.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPhysicalDevice.h"

class VulkanManager
{
public:
    VulkanManager();
    ~VulkanManager();

    VulkanManager(VulkanManager&) = delete;
    VulkanManager(const VulkanManager&) = delete;
    VulkanManager(VulkanManager&&) = delete;
    VulkanManager(const VulkanManager&&) = delete;

private:
    // Vulkan objects following RAII pattern
    const std::shared_ptr<VulkanInstance> m_instance = std::make_shared<VulkanInstance>();
    const VulkanLogger m_logger = { m_instance };
    const std::shared_ptr<VulkanPhysicalDevice> m_physicalDevice = std::make_shared<VulkanPhysicalDevice>(m_instance);
    const VulkanLogicalDevice m_logicalDevice = { m_physicalDevice };
};
#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanInstance.h"
#include "VulkanLogger.h"

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
    const std::shared_ptr<VulkanInstance> m_instance = std::make_shared<VulkanInstance>();
    VulkanLogger m_logger = { m_instance };
};
#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
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

    void initVulkan();

private:
    void createInstance();
    std::vector<const char*> getExtensions();
    std::vector<const char*> getValidationLayers();

    VkInstance m_instance;
    std::unique_ptr<VulkanLogger> m_logger;
};
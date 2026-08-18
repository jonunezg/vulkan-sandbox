#include <set>

#include "VulkanExtensions.h"
#include "VulkanLayers.h"
#include "VulkanLogicalDevice.h"

VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice& physicalDevice)
{
    const float priority = 1.0f;
    const auto& device = physicalDevice.getSelectedDevice();

    std::set<uint32_t> indices =
    {
        device.graphicQueueIndex.value(),
        device.presentQueueIndex.value()
    };

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfo;

    for (const auto& index : indices)
    {
        VkDeviceQueueCreateInfo createInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = index,
            .queueCount = 1,
            .pQueuePriorities = &priority,
        };
        queueCreateInfo.push_back(createInfo);
    };

    const VkPhysicalDeviceFeatures deviceFeatures {};
    const auto requiredExtensions = getRequiredPhysicalDeviceExtensions();

    const VkDeviceCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfo.size()),
        .pQueueCreateInfos = queueCreateInfo.data(),
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
        .pEnabledFeatures = &deviceFeatures,
    };

    VK_THROW_IF_FAILED(vkCreateDevice(device.device, &createInfo, nullptr, &m_device));

    vkGetDeviceQueue(m_device, device.graphicQueueIndex.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, device.presentQueueIndex.value(), 0, &m_presentQueue);

    LOG_ENGINE_INFO("Vulkan logical device created: " << m_device);
    LOG_ENGINE_INFO("Graphics queue: " << m_graphicsQueue);
    LOG_ENGINE_INFO("Present queue: " << m_presentQueue);
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
    LOG_ENGINE_INFO("Vulkan logical device destroyed: " << m_device);
}
#include "VulkanLayers.h"
#include "VulkanLogicalDevice.h"

VulkanLogicalDevice::VulkanLogicalDevice(const std::shared_ptr<VulkanPhysicalDevice> physicalDevice) :
m_physicalDevice { std::move(physicalDevice) }
{
    if (!m_physicalDevice)
    {
        throw std::runtime_error("Logical device created without physical device");
    }

    const float priority = 1.0f;

    const VkDeviceQueueCreateInfo queueCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueFamilyIndex = m_physicalDevice->getSelectedDevice().graphicQueueIndex.value(),
        .queueCount = 1,
        .pQueuePriorities = &priority,
    };

    const VkPhysicalDeviceFeatures deviceFeatures {};

    const VkDeviceCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueCreateInfo,
        .enabledLayerCount = 0,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = 0,
        .ppEnabledExtensionNames = nullptr,
        .pEnabledFeatures = &deviceFeatures,
    };

    VK_TERMINATE_IF_FAILED(vkCreateDevice(m_physicalDevice->getSelectedDevice().device, &createInfo, nullptr, &m_device));

    VkQueue graphicsQueue;

    vkGetDeviceQueue(m_device, m_physicalDevice->getSelectedDevice().graphicQueueIndex.value(), 0, &graphicsQueue);
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
    std::cout << "Vulkan logical device destroyed" << std::endl;
}
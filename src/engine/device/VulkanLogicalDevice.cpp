#include <set>

#include "VulkanExtensions.h"
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

    std::set<uint32_t> indices =
    {
        m_physicalDevice->getSelectedDevice().graphicQueueIndex.value(),
        m_physicalDevice->getSelectedDevice().presentQueueIndex.value()
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

    VK_THROW_IF_FAILED(vkCreateDevice(m_physicalDevice->getSelectedDevice().device, &createInfo, nullptr, &m_device));

    vkGetDeviceQueue(m_device, m_physicalDevice->getSelectedDevice().graphicQueueIndex.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, m_physicalDevice->getSelectedDevice().presentQueueIndex.value(), 0, &m_presentQueue);

    std::cout << "Graphics queue: " << m_graphicsQueue << std::endl;
    std::cout << "Present queue: " << m_presentQueue << std::endl;
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    vkDestroyDevice(m_device, nullptr);
    std::cout << "Vulkan logical device destroyed" << std::endl;
}
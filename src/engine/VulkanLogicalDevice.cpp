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
        .pQueuePriorities = &priority
    };

    const VkPhysicalDeviceFeatures deviceFeatures {};



}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    std::cout << "Vulkan logical device destroyed" << std::endl;
}
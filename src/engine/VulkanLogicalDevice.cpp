#include "VulkanLogicalDevice.h"

VulkanLogicalDevice::VulkanLogicalDevice(const std::shared_ptr<VulkanPhysicalDevice> physicalDevice)
{
    if (!physicalDevice)
    {
        throw std::runtime_error("Logical device created without physical device");
    }
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    std::cout << "Vulkan logical device destroyed" << std::endl;
}
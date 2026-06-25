#include <vector>

#include "VulkanPhysicalDevice.h"

// Local helpers

void dumpPhysicalDevices(const std::vector<VkPhysicalDevice>& devices)
{
    std::cout << "Physical devices found" << std::endl;

    for(const auto& device : devices)
    {
        VkPhysicalDeviceProperties properties;

        vkGetPhysicalDeviceProperties(device, &properties);
        std::cout << "\t" << properties.deviceName << std::endl;
    }
}

// Class methods

std::vector<VkPhysicalDevice> VulkanPhysicalDevice::getPhysicalDevices()
{
    uint32_t deviceCount = 0;

    VK_TERMINATE_IF_FAILED(vkEnumeratePhysicalDevices(m_instance->getInstance(), &deviceCount, nullptr));
    
    if (deviceCount == 0)
    {
        throw std::runtime_error("No available devices that support Vulkan");
    }

    std::vector<VkPhysicalDevice> devices { deviceCount };
    
    VK_TERMINATE_IF_FAILED(vkEnumeratePhysicalDevices(m_instance->getInstance(), &deviceCount, devices.data()));

    if (VK_ENABLE_DEBUG)
    {
        dumpPhysicalDevices(devices);
    }

    return devices;
}

VulkanPhysicalDevice::VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance) :
m_instance { std::move(instance) }
{
    const auto devices = getPhysicalDevices();
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}
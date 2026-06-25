#include <vector>

#include "VulkanPhysicalDevice.h"


// Local helpers

void dumpPhysicalDevices(const std::vector<PhysicalDevice>& devices)
{
    std::cout << "Physical devices found" << std::endl;

    for(const auto& device : devices)
    {
        VkPhysicalDeviceProperties properties;

        std::cout << "\t" << device.properties.deviceName << std::endl;
    }
}

bool isDeviceSuitable(const PhysicalDevice& device)
{
    return device.features.geometryShader;
}

const PhysicalDevice& selectDevice(const std::vector<PhysicalDevice>& devices)
{
    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            return device;
        }
    }
    throw std::runtime_error("No suitable device");
}

// Class methods

std::vector<PhysicalDevice> VulkanPhysicalDevice::getPhysicalDevices()
{
    uint32_t deviceCount = 0;

    VK_TERMINATE_IF_FAILED(vkEnumeratePhysicalDevices(m_instance->getInstance(), &deviceCount, nullptr));
    
    if (deviceCount == 0)
    {
        throw std::runtime_error("No available devices that support Vulkan");
    }

    std::vector<VkPhysicalDevice> devices { deviceCount };
    
    VK_TERMINATE_IF_FAILED(vkEnumeratePhysicalDevices(m_instance->getInstance(), &deviceCount, devices.data()));

    std::vector<PhysicalDevice> devicesInfo { deviceCount };
    auto i = devicesInfo.begin();

    for (const auto& device : devices)
    {
        i->device = device;
        // Ignore errors in obtaining information about devices
        vkGetPhysicalDeviceProperties(device, &i->properties);
        vkGetPhysicalDeviceFeatures(device, &i->features);
        i++;
    }

    return devicesInfo;
}

VulkanPhysicalDevice::VulkanPhysicalDevice(std::shared_ptr<VulkanInstance> instance) :
m_instance { std::move(instance) }
{
    const auto devices = getPhysicalDevices();

    if (VK_ENABLE_DEBUG)
    {
        dumpPhysicalDevices(devices);
    }

    const auto& selectedDevice = selectDevice(devices);
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}
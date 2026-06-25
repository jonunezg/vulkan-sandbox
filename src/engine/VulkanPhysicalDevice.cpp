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
    bool hasGraphicsFamily = false;

    for (const auto& queueFamily : device.queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            hasGraphicsFamily = true;
        }
    }

    return hasGraphicsFamily && device.features.geometryShader;
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

        uint32_t queueFaimilyCount = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFaimilyCount, nullptr);

        if (queueFaimilyCount)
        {
            i->queueFamilies = std::vector<VkQueueFamilyProperties>{ queueFaimilyCount };
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFaimilyCount, i->queueFamilies.data());
        }

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

    m_physicalDevice = selectDevice(devices).device;

    if (m_physicalDevice != VK_NULL_HANDLE)
    {
        throw std::runtime_error("Unable to select an appropriate physical device");
    }
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{}
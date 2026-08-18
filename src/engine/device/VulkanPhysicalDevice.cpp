#include <vector>

#include "VulkanExtensions.h"
#include "VulkanLogger.h"
#include "VulkanPhysicalDevice.h"


// Local helpers

void dumpPhysicalDevices(const std::vector<PhysicalDevice>& devices)
{
    LOG_ENGINE_INFO("Physical devices found");

    for(const auto& device : devices)
    {
        VkPhysicalDeviceProperties properties;

        LOG_ENGINE_INFO("\t" << device.properties.deviceName);
    }
}

bool deviceSupportsRequiredExtensions(const PhysicalDevice& device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device.device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions { extensionCount };
    vkEnumerateDeviceExtensionProperties(device.device, nullptr, &extensionCount, availableExtensions.data());

    const auto requiredExtensionsVector = getRequiredPhysicalDeviceExtensions();
    auto requiredExtensions = std::set<std::string>{ requiredExtensionsVector.begin(), requiredExtensionsVector.end() };

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

bool deviceSupportsSwapchain(const PhysicalDevice& device)
{
    bool support = false;
    if (deviceSupportsRequiredExtensions(device))
    {
        support = !device.formats.empty() && !device.presentModes.empty();
    }
    return support;
}

bool isDeviceSuitable(const PhysicalDevice& device)
{
    return device.presentQueueIndex
        && device.graphicQueueIndex
        && device.features.geometryShader
        && deviceSupportsSwapchain(device);
}

const PhysicalDevice& selectDevice(const std::vector<PhysicalDevice>& devices)
{
    for (const auto& device : devices)
    {
        if (isDeviceSuitable(device))
        {
            LOG_ENGINE_INFO("Selected physical device: " << device.properties.deviceName);
            return device;
        }
    }
    throw std::runtime_error("No suitable device");
}

void VulkanPhysicalDevice::processQueueFamilies(PhysicalDevice& device)
{
    uint32_t i = 0;

    for (const auto& queueFamily : device.queueFamilies)
    {
        if (!device.graphicQueueIndex && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
        {
            device.graphicQueueIndex = i;
        }

        if(!device.presentQueueIndex)
        {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device.device, i, m_surface.getSurface(), &presentSupport);
            if (presentSupport)
            {
                device.presentQueueIndex = i;
            }
        }

        i++;
    }
}

// Class methods

std::vector<PhysicalDevice> VulkanPhysicalDevice::getPhysicalDevices()
{
    uint32_t deviceCount = 0;

    VK_THROW_IF_FAILED(vkEnumeratePhysicalDevices(m_instance.getInstance(), &deviceCount, nullptr));
    
    if (deviceCount == 0)
    {
        throw std::runtime_error("No available devices that support Vulkan");
    }

    std::vector<VkPhysicalDevice> devices { deviceCount };
    
    VK_THROW_IF_FAILED(vkEnumeratePhysicalDevices(m_instance.getInstance(), &deviceCount, devices.data()));

    std::vector<PhysicalDevice> devicesInfo { deviceCount };
    auto i = devicesInfo.begin();

    for (const auto& device : devices)
    {
        if (i == devicesInfo.end())
        {
            throw std::runtime_error("Reached the end of the devices vector");
        }

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
            processQueueFamilies(*i);
        }

        if (deviceSupportsRequiredExtensions(*i))
        {
            UpdateSwapchainSupportSettings(m_surface.getSurface(), *i);
        }

        i++;
    }

    return devicesInfo;
}

void VulkanPhysicalDevice::UpdateSwapchainSupportSettings(const VkSurfaceKHR& surface, PhysicalDevice& storage)
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(storage.device, surface, &storage.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(storage.device, surface, &formatCount, nullptr);
    
    if (formatCount)
    {
        storage.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(storage.device, surface, &formatCount, storage.formats.data());

    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(storage.device, surface, &presentModeCount, nullptr);

    if (presentModeCount)
    {
        storage.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(storage.device, surface, &presentModeCount, storage.presentModes.data());
    }
}

VulkanPhysicalDevice::VulkanPhysicalDevice(
    const VulkanInstance& instance,
    const VulkanSurface& surface) :
m_instance { std::move(instance) },
m_surface { std::move(surface) }
{
    const auto devices = getPhysicalDevices();

    if (VK_ENABLE_DEBUG)
    {
        dumpPhysicalDevices(devices);
    }

    m_selectedDevice = selectDevice(devices);
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}
#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLayers.h"

std::vector<VkLayerProperties> getAvailableValidationLayers()
{
    uint32_t count = 0;
    VK_TERMINATE_IF_FAILED(vkEnumerateInstanceLayerProperties(&count, nullptr));

    std::vector<VkLayerProperties> properties(count);
    VK_TERMINATE_IF_FAILED(vkEnumerateInstanceLayerProperties(&count, properties.data()));

    return properties;
}

void dumpAvailableValidationLayers()
{
    const auto layers = getAvailableValidationLayers();
    std::cout << "Available Vulkan validation layers:" << std::endl;
    for(const auto& layer : layers)
    {
        std::cout << "\t" << layer.layerName << ": " << layer.description << std::endl;
    }
}

void dumpRequiredValidationLayers(const std::vector<const char*>& layers)
{
    std::cout << "Required " << layers.size() << " Vulkan validation layers:" << std::endl;
    for (const auto& layer : layers)
    {
        std::cout << "\t" << layer << std::endl;
    }
}

std::vector<const char*> getValidationLayers()
{
    if (!VK_ENABLE_DEBUG)
    {
        return {};
    }

    std::vector<const char*> layers = VK_REQUIRED_VALIDATION_LAYERS;
    dumpAvailableValidationLayers();
    dumpRequiredValidationLayers(layers);

    return layers;
}
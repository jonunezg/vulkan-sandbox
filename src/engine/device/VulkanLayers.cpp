#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLayers.h"
#include "VulkanLogger.h"

std::vector<VkLayerProperties> getAvailableValidationLayers()
{
    uint32_t count = 0;
    VK_THROW_IF_FAILED(vkEnumerateInstanceLayerProperties(&count, nullptr));

    std::vector<VkLayerProperties> properties(count);
    VK_THROW_IF_FAILED(vkEnumerateInstanceLayerProperties(&count, properties.data()));

    return properties;
}

void dumpAvailableValidationLayers()
{
    const auto layers = getAvailableValidationLayers();
    LOG_ENGINE_VERBOSE("Available Vulkan validation layers:");
    for(const auto& layer : layers)
    {
        LOG_ENGINE_VERBOSE("\t" << layer.layerName << ": " << layer.description);
    }
}

void dumpRequiredValidationLayers(const std::vector<const char*>& layers)
{
    LOG_ENGINE_INFO("Required " << layers.size() << " Vulkan validation layers:");
    for (const auto& layer : layers)
    {
        LOG_ENGINE_INFO("\t" << layer);
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
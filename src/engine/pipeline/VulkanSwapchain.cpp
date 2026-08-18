#include "algorithm"

#include "VulkanSwapchain.h"

VkSurfaceFormatKHR selectSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
    for (const auto& format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }

    return formats[0];
}

VkPresentModeKHR selectPresentMode(const std::vector<VkPresentModeKHR>& modes)
{
    for (const auto& mode : modes)
    {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR; // This mode is the only one guaranteed to exist
}

VkExtent2D VulkanSwapchain::selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        // Vulkan already figured out the extent size
        return capabilities.currentExtent;
    }
    else
    {
        // Get the extent size using GLFW
        const auto bufferSize = m_windowManager.getFrameBufferSize();

        return
        {
            std::clamp(bufferSize.first, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(bufferSize.second, capabilities.minImageExtent.height, capabilities.minImageExtent.height)
        };
    }
}

void VulkanSwapchain::createImageViews()
{
    m_imageViews.reserve(m_images.size());
    for (const auto& image : m_images)
    {
        m_imageViews.emplace_back(m_device, m_format, image);
    }
}

VulkanSwapchain::VulkanSwapchain(
    const PhysicalDevice& physicalDevice,
    const VkDevice& logicalDevice,
    const VkSurfaceKHR& surface,
    const WindowManager& windowManager) :
m_device { logicalDevice },
m_windowManager { std::move(windowManager) },
m_format { selectSwapchainFormat(physicalDevice.formats) },
m_extent { selectSwapExtent(physicalDevice.capabilities) }
{
    const auto mode = selectPresentMode(physicalDevice.presentModes);
    const uint32_t imageCount = physicalDevice.capabilities.maxImageCount > 0
        ? std::clamp(physicalDevice.capabilities.minImageCount + 1, physicalDevice.capabilities.minImageCount, physicalDevice.capabilities.maxImageCount)
        : physicalDevice.capabilities.minImageCount + 1;
    const bool multiFamily = physicalDevice.graphicQueueIndex != physicalDevice.presentQueueIndex;
    const uint32_t familyIndices[] = { physicalDevice.graphicQueueIndex.value(), physicalDevice.presentQueueIndex.value() };

    const VkSwapchainCreateInfoKHR createInfo
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = m_format.format,
        .imageColorSpace = m_format.colorSpace,
        .imageExtent = m_extent,
        .imageArrayLayers = 1, // Single layer, multiple layers are used for stereoscopic applications
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, // Direct drawing, other values are used for post processing
        .imageSharingMode = multiFamily ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = multiFamily ? 2u : 0u,
        .pQueueFamilyIndices = multiFamily ? familyIndices : nullptr,
        .preTransform = physicalDevice.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = mode,
        .clipped = VK_TRUE, // Don't care about pixels obscured by other windows
        .oldSwapchain = VK_NULL_HANDLE,
    };

    VK_THROW_IF_FAILED(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain))

    uint32_t swapchainImageCount;
    VK_THROW_IF_FAILED(vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, nullptr));
    
    if (!swapchainImageCount)
    {
        throw std::runtime_error("Swapchain lacks images");
    }
    
    m_images.resize(swapchainImageCount);
    VK_THROW_IF_FAILED(vkGetSwapchainImagesKHR(m_device, m_swapchain, &swapchainImageCount, m_images.data()));

    createImageViews();

    LOG_ENGINE_INFO("Vulkan swapchain created: " << m_swapchain);
}

VulkanSwapchain::~VulkanSwapchain()
{
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

    LOG_ENGINE_INFO("Vulkan swapchain destroyed: " << m_swapchain);
}
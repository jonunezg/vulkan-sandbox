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
        const auto bufferSize = m_windowManager->getFrameBufferSize();

        return
        {
            std::clamp(bufferSize.first, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(bufferSize.second, capabilities.minImageExtent.height, capabilities.minImageExtent.height)
        };
    }
}

void VulkanSwapchain::createImageViews()
{
    m_imageViews.resize(m_images.size());

    for (size_t i = 0; i < m_images.size(); i++)
    {
        const VkImageViewCreateInfo createInfo
        {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = m_images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_format.format,
            .components =
            {
                VK_COMPONENT_SWIZZLE_IDENTITY,  // r
                VK_COMPONENT_SWIZZLE_IDENTITY,  // g
                VK_COMPONENT_SWIZZLE_IDENTITY,  // b
                VK_COMPONENT_SWIZZLE_IDENTITY   // a
            },
            .subresourceRange =
            {
                VK_IMAGE_ASPECT_COLOR_BIT,  // aspect mask
                0,                          // base mip level
                1,                          // level count
                0,                          // base array layer
                1                           // layer count
            }
        };

        VK_TERMINATE_IF_FAILED(vkCreateImageView(m_logicalDevice->getDevice(), &createInfo, nullptr, &m_imageViews[i]));
    }
}

VulkanSwapchain::VulkanSwapchain(
    const std::shared_ptr<VulkanPhysicalDevice> physicalDevice,
    const std::shared_ptr<VulkanLogicalDevice> logicalDevice,
    const std::shared_ptr<VulkanSurface> surface,
    const std::shared_ptr<WindowManager> windowManager) :
m_physicalDevice { std::move(physicalDevice) },
m_logicalDevice { std::move(logicalDevice) },
m_surface { std::move(surface) },
m_windowManager { std::move(windowManager) },
m_format { selectSwapchainFormat(m_physicalDevice->getSelectedDevice().formats) },
m_extent { selectSwapExtent(m_physicalDevice->getSelectedDevice().capabilities) }
{
    if (!m_physicalDevice)
    {
        throw std::runtime_error("Swapchain created without physical device");
    }

    if (!m_logicalDevice)
    {
        throw std::runtime_error("Swapchain created without logical device");
    }

    if (!m_surface)
    {
        throw std::runtime_error("Swapchain created without surface");
    }

    if (!m_windowManager)
    {
        throw std::runtime_error("Swapchain created without window manager");
    }

    const auto device = m_physicalDevice->getSelectedDevice();

    const auto mode = selectPresentMode(device.presentModes);
    const uint32_t imageCount = device.capabilities.maxImageCount > 0
        ? std::clamp(device.capabilities.minImageCount + 1, device.capabilities.minImageCount, device.capabilities.maxImageCount)
        : device.capabilities.minImageCount + 1;
    const bool multiFamily = device.graphicQueueIndex != device.presentQueueIndex;
    const uint32_t familyIndices[] = { device.graphicQueueIndex.value(), device.presentQueueIndex.value() };

    const VkSwapchainCreateInfoKHR createInfo
    {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .surface = m_surface->getSurface(),
        .minImageCount = imageCount,
        .imageFormat = m_format.format,
        .imageColorSpace = m_format.colorSpace,
        .imageExtent = m_extent,
        .imageArrayLayers = 1, // Single layer, multiple layers are used for stereoscopic applications
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, // Direct drawing, other values are used for post processing
        .imageSharingMode = multiFamily ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = multiFamily ? 2u : 0u,
        .pQueueFamilyIndices = multiFamily ? familyIndices : nullptr,
        .preTransform = device.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = mode,
        .clipped = VK_TRUE, // Don't care about pixels obscured by other windows
        .oldSwapchain = VK_NULL_HANDLE,
    };

    VK_TERMINATE_IF_FAILED(vkCreateSwapchainKHR(m_logicalDevice->getDevice(), &createInfo, nullptr, &m_swapchain))

    uint32_t swapchainImageCount;
    VK_TERMINATE_IF_FAILED(vkGetSwapchainImagesKHR(m_logicalDevice->getDevice(), m_swapchain, &swapchainImageCount, nullptr));
    
    if (!swapchainImageCount)
    {
        throw std::runtime_error("Swapchain lacks images");
    }
    
    m_images.resize(swapchainImageCount);
    VK_TERMINATE_IF_FAILED(vkGetSwapchainImagesKHR(m_logicalDevice->getDevice(), m_swapchain, &swapchainImageCount, m_images.data()));

    createImageViews();

    std::cout << "Vulkan swapchain created" << std::endl;
}

VulkanSwapchain::~VulkanSwapchain()
{
    for (const auto& imageView : m_imageViews)
    {
        vkDestroyImageView(m_logicalDevice->getDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(m_logicalDevice->getDevice(), m_swapchain, nullptr);

    std::cout << "Vulkan swapchain destroyed" << std::endl;
}
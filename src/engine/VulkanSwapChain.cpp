#include "VulkanSwapChain.h"

VkSurfaceFormatKHR selectSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats)
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

VkExtent2D VulkanSwapChain::selectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
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
    }
}

VulkanSwapChain::VulkanSwapChain(
    const std::shared_ptr<VulkanPhysicalDevice> physicalDevice,
    const std::shared_ptr<VulkanSurface> surface,
    const std::shared_ptr<WindowManager> windowManager) :
m_physicalDevice { std::move(physicalDevice) },
m_surface { std::move(surface) },
m_windowManager { std::move(windowManager) }
{
    if (!m_physicalDevice)
    {
        throw std::runtime_error("Swap chain created without physical device");
    }

    if (!m_surface)
    {
        throw std::runtime_error("Swap chain created without surface");
    }

    if (!m_windowManager)
    {
        throw std::runtime_error("Swap chain created without window manager");
    }

    auto format = selectSwapChainFormat(m_physicalDevice->getSelectedDevice().formats);
    auto mode = selectPresentMode(m_physicalDevice->getSelectedDevice().presentModes);
}

VulkanSwapChain::~VulkanSwapChain()
{
}
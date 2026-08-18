#include "VulkanSurface.h"

VulkanSurface::VulkanSurface(
    const WindowManager& windowManager,
    const std::shared_ptr<VulkanInstance> instance) :
m_instance { std::move(instance) }
{
    if (!m_instance)
    {
        throw std::runtime_error("Surface created without Vulkan instance");
    }

    VK_THROW_IF_FAILED(glfwCreateWindowSurface(m_instance->getInstance(), windowManager.getWindowHandle(), nullptr, &m_surface));

    LOG_ENGINE_INFO("Vulkan surface created: " << m_surface);
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance->getInstance(), m_surface, nullptr);

    LOG_ENGINE_INFO("Vulkan surface destroyed: " << m_surface);
}
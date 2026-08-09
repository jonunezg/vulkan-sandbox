#include "VulkanSurface.h"

VulkanSurface::VulkanSurface(
    const std::shared_ptr<WindowManager> windowManager,
    const std::shared_ptr<VulkanInstance> instance) :
m_windowManager { std::move(windowManager) },
m_instance { std::move(instance) }
{
    if (!m_windowManager || !m_instance)
    {
        throw std::runtime_error("Surface created without window manager or Vulkan instance");
    }

    VK_THROW_IF_FAILED(glfwCreateWindowSurface(m_instance->getInstance(), m_windowManager->getWindowHandle(), nullptr, &m_surface));

    LOG_ENGINE_INFO("Vulkan surface created");
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance->getInstance(), m_surface, nullptr);

    LOG_ENGINE_INFO("Vulkan surface destroyed");
}
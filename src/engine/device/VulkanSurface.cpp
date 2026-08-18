#include "VulkanSurface.h"

VulkanSurface::VulkanSurface(
    GLFWwindow* window,
    const VkInstance& instance) :
m_instance { instance }
{
    VK_THROW_IF_FAILED(glfwCreateWindowSurface(m_instance, window, nullptr, &m_surface));

    LOG_ENGINE_INFO("Vulkan surface created: " << m_surface);
}

VulkanSurface::~VulkanSurface()
{
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

    LOG_ENGINE_INFO("Vulkan surface destroyed: " << m_surface);
}
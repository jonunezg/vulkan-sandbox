#pragma once

#include "GlfwManager.h"

class WindowManager
{
public:
    WindowManager()
    {
        if (m_glfwManager.glfwOk())
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // Don't start OpenGL
            // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // No resizable window

            LOG_ENGINE_INFO("Creating window");
            m_window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "Vulkan app", nullptr, nullptr);
            LOG_ENGINE_INFO("Window created");

            if(m_window == nullptr)
            {
                throw std::runtime_error("Unable to create window");
            }
        }
    }

    ~WindowManager()
    {
        if (m_window) {
            glfwDestroyWindow(m_window);
            LOG_ENGINE_INFO("Window destroyed");
        }
    }

    WindowManager(const WindowManager&) = delete;
    WindowManager(WindowManager&) = delete;
    WindowManager(const WindowManager&&) = delete;
    WindowManager(WindowManager&&) = delete;

    bool windowShouldClose() const
    {
        if (!m_glfwManager.glfwOk() || !m_window) {
            return true;
        }

        return glfwWindowShouldClose(m_window);
    }

    GLFWwindow* getWindowHandle() const { return m_window; }

    const std::pair<uint32_t, uint32_t> getFrameBufferSize()
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        return { width, height };
    }

private:
    GlfwManager m_glfwManager;
    GLFWwindow* m_window = nullptr;

};
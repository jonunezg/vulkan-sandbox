#pragma once

#include <atomic>

#include "GlfwManager.h"

static void resizeCallback(GLFWwindow* window, int, int);

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

            glfwSetWindowUserPointer(m_window, this);
            glfwSetFramebufferSizeCallback(m_window, resizeCallback);
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

    const std::pair<uint32_t, uint32_t> getFrameBufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        return { width, height };
    }

    bool windowResized() { bool resized = true; return m_windowResized.compare_exchange_strong(resized, false); }
    void setWindowResized() { m_windowResized.store(true); }
    void pauseWhileMinimized()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(m_window, &width, &height);
        bool paused = width == 0 || height == 0;
        if (paused)
        {
            LOG_ENGINE_WARNING("Window minimized, pausing");
        }
        while (width == 0 || height == 0)
        {
            glfwWaitEvents();
            glfwGetFramebufferSize(m_window, &width, &height);
        }
        if (paused)
        {
            LOG_ENGINE_WARNING("Window no longer minimized, continuing");
        }
    }

private:
    GlfwManager m_glfwManager;
    GLFWwindow* m_window = nullptr;

    std::atomic<bool> m_windowResized = false;
};

static void resizeCallback(GLFWwindow* window, int, int)
{
    auto manager = reinterpret_cast<WindowManager*>(glfwGetWindowUserPointer(window));
    manager->setWindowResized();
}
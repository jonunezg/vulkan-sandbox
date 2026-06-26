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
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // No resizable window

            std::cout << "Creating window" << std::endl;
            m_window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_WIDTH, "Vulkan app", nullptr, nullptr);
            std::cout << "Window created" << std::endl;

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
            std::cout << "Window destroyed" << std::endl;
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

private:
    GlfwManager m_glfwManager;
    GLFWwindow* m_window = nullptr;

};
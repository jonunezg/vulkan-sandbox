#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "engine/VulkanManager.h"

class GlfwManager
{
public:
    GlfwManager() :
    m_glfwStatus { glfwInit() }
    {
        std::cout << "GLFW initialized" << std::endl;
    }

    ~GlfwManager()
    {
        if (glfwOk())
        {
            glfwTerminate();
            std::cout << "GLFW terminated" << std::endl;
        }
    }

    GlfwManager(GlfwManager&) = delete;
    GlfwManager(const GlfwManager&) = delete;
    GlfwManager(GlfwManager&&) = delete;
    GlfwManager(const GlfwManager&&) = delete;

    bool glfwOk()
    {
        return m_glfwStatus == GLFW_TRUE;
    }

private:
    int m_glfwStatus;
};

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

            assert(m_window != nullptr);
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

    bool windowShouldClose()
    {
        if (!m_glfwManager.glfwOk() || !m_window) {
            return true;
        }

        return glfwWindowShouldClose(m_window);
    }
private:
    GlfwManager m_glfwManager;
    GLFWwindow* m_window = nullptr;

};


class HelloTriangleApplication
{
public:
    void run()
    {
        init();
        std::cout << BLUE << "Application started successfully" << RESET << std::endl;
        mainLoop();
        cleanup();
    }

private:
    void init()
    {
        m_vulkanManager.initVulkan();
    }

    void mainLoop()
    {    
        while (!m_windowManager.windowShouldClose())
        {
            glfwPollEvents();
        }
    }

    void cleanup() {        
    }

    WindowManager m_windowManager;
    VulkanManager m_vulkanManager;
};

int main()
{
    {
        auto app = HelloTriangleApplication {};
    
        try
        {
            app.run();
            glfwTerminate();
        }
        catch (const std::exception& e)
        {
            std::cerr << RED << e.what() << RESET << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << GREEN << "Application completed succesfully" << RESET << std::endl;
    return EXIT_SUCCESS;
}
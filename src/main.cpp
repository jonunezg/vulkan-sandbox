#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "engine/VulkanManager.h"

class HelloTriangleApplication
{
public:
    void run()
    {
        m_vulkanManager.LoadShader("triangle_vert.spv");
        m_vulkanManager.LoadShader("triangle_frag.spv");
        std::cout << BLUE << "Application started successfully" << RESET << std::endl;
        mainLoop();
        cleanup();
    }

private:

    void mainLoop()
    {    
        while (!m_vulkanManager.shouldClose())
        {
            glfwPollEvents();
        }
    }

    void cleanup() {        
    }

    VulkanManager m_vulkanManager;
};

int main()
{
    {
        
        try
        {
            auto app = HelloTriangleApplication {};
            app.run();
            glfwTerminate();
        }
        catch (const std::exception& e)
        {
            std::cerr << RED << "[Error]: " << e.what() << RESET << std::endl;
            return EXIT_FAILURE;
        }
    }

    std::cout << GREEN << "Application completed succesfully" << RESET << std::endl;
    return EXIT_SUCCESS;
}
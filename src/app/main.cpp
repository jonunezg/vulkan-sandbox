#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "VulkanPipelineManager.h"

class HelloTriangleApplication
{
public:
    void run()
    {
        m_vulkanPipelineManager.LoadShader("triangle_vert.spv", ShaderType::Vertex);
        m_vulkanPipelineManager.LoadShader("triangle_frag.spv", ShaderType::Fragment);
        std::cout << BLUE << "Application started successfully" << RESET << std::endl;
        mainLoop();
        cleanup();
    }

private:

    void mainLoop()
    {    
        while (!m_vulkanPipelineManager.shouldClose())
        {
            glfwPollEvents();
        }
    }

    void cleanup() {        
    }

    VulkanPipelineManager m_vulkanPipelineManager;
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
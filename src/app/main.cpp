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

    VulkanPipelineManager m_vulkanPipelineManager {std::vector<Shader>{ { ShaderType::Vertex, "triangle_vert.spv" }, { ShaderType::Fragment , "triangle_frag.spv" } }};
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
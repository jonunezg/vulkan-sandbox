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
        LOG_APP_VERBOSE("Application started successfully"s);
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
            LOG_APP_ERROR("[Error]: " << e.what());
            return EXIT_FAILURE;
        }
    }

    LOG_APP_VERBOSE("Application completed succesfully"s);
    return EXIT_SUCCESS;
}
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
        while (m_vulkanPipelineManager.drawFrame())
        {
            glfwPollEvents();
        }
    }

    void cleanup() {        
    }

    VulkanPipelineManager m_vulkanPipelineManager
    {
        std::vector<Shader>{ { ShaderType::Vertex, "triangle_vert.spv" }, { ShaderType::Fragment , "triangle_frag.spv" } },
        std::vector<Geometry::Vertex>
        {
            { { 0.0f, -0.5f }, { 1.0f, 0.0f, 1.0f } },
            { { 0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f } },
            { {-0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f } }
        }
    };
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
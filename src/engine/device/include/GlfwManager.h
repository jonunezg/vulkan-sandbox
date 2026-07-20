#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"

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

    bool glfwOk() const
    {
        return m_glfwStatus == GLFW_TRUE;
    }

private:
    int m_glfwStatus;
};
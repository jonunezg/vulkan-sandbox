#pragma once

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"

class GlfwManager
{
public:
    GlfwManager() :
    m_glfwStatus { glfwInit() }
    {
        LOG_ENGINE_INFO("GLFW initialized");
    }

    ~GlfwManager()
    {
        if (glfwOk())
        {
            glfwTerminate();
            LOG_ENGINE_INFO("GLFW terminated");
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
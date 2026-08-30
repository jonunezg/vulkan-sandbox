#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanVertexInput.h"

class VulkanBuffer
{
public:
    VulkanBuffer(const PhysicalDevice& physicalDevice, const VkDevice& device, const std::vector<Geometry::Vertex>& vertices);

    // VulkanBuffer(VulkanBuffer& other) :
    // m_physicalDevice { other.m_physicalDevice },
    // m_device { other.m_device }
    // {
    //     m_vertexBuffer = other.m_vertexBuffer;
    //     other.m_vertexBuffer = nullptr;

    //     m_vertexBufferMemory = other.m_vertexBufferMemory;
    //     other.m_vertexBufferMemory = nullptr;
    // }
    
    VulkanBuffer(const VulkanBuffer&) = delete;

    // VulkanBuffer(VulkanBuffer&& other) :
    // m_physicalDevice { other.m_physicalDevice },
    // m_device { other.m_device }
    // {
    //     m_vertexBuffer = other.m_vertexBuffer;
    //     other.m_vertexBuffer = nullptr;

    //     m_vertexBufferMemory = other.m_vertexBufferMemory;
    //     other.m_vertexBufferMemory = nullptr;
    // }

    VulkanBuffer(const VulkanBuffer&&) = delete;

    ~VulkanBuffer();

    const VkBuffer& getBuffer() const { return m_vertexBuffer; }

private:
    const PhysicalDevice& m_physicalDevice;
    const VkDevice& m_device;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
};
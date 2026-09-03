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
    VulkanBuffer(
        const PhysicalDevice& physicalDevice,
        const VkDevice& device,
        const VkDeviceSize size,
        const VkBufferUsageFlags usage,
        const VkMemoryPropertyFlags properties);

    VulkanBuffer(VulkanBuffer& other) = delete;
    VulkanBuffer(const VulkanBuffer&) = delete;
    VulkanBuffer(VulkanBuffer&& other) = delete;
    VulkanBuffer(const VulkanBuffer&&) = delete;

    ~VulkanBuffer();

    void copyBuffer(const VulkanBuffer& src);

    const VkBuffer& getBuffer() const { return m_vertexBuffer; }
    const VkDeviceMemory& getBufferMemory() const { return m_vertexBufferMemory; }
    
private:
    const PhysicalDevice& m_physicalDevice;
    const VkDevice& m_device;
    VkBuffer m_vertexBuffer;
    VkDeviceMemory m_vertexBufferMemory;
};
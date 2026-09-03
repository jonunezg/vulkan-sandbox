#pragma once

#include <vector>

#include "Utilities.h"
#include "VulkanBuffer.h"
#include "VulkanIncludes.h"
#include "VulkanTransferCommandBuffer.h"

class VulkanVertexBuffer
{
public:
    VulkanVertexBuffer(
        const PhysicalDevice& physicalDevice,
        const VkDevice& device,
        const VkQueue& queue,
        const VkCommandPool& commandPool,
        const std::vector<Geometry::Vertex>& vertices);

    VulkanVertexBuffer(VulkanVertexBuffer& other) = delete;
    VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
    VulkanVertexBuffer(VulkanVertexBuffer&& other) = delete;
    VulkanVertexBuffer(const VulkanVertexBuffer&&) = delete;

    ~VulkanVertexBuffer();

    const VkBuffer& getBuffer() const { return m_vertexBuffer.getBuffer(); }

private:
    const PhysicalDevice& m_physicalDevice;
    const VkDevice& m_device;
    VulkanTransferCommandBuffer m_transferCommandBuffer;
    VulkanBuffer m_vertexBuffer;
};
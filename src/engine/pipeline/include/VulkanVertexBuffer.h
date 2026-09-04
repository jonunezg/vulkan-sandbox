#pragma once

#include <optional>
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
        const std::vector<Geometry::Vertex>& vertices,
        const std::optional<std::vector<uint16_t>> indices);

    VulkanVertexBuffer(VulkanVertexBuffer& other) = delete;
    VulkanVertexBuffer(const VulkanVertexBuffer&) = delete;
    VulkanVertexBuffer(VulkanVertexBuffer&& other) = delete;
    VulkanVertexBuffer(const VulkanVertexBuffer&&) = delete;

    ~VulkanVertexBuffer();

    const VkBuffer& getBuffer() const { return m_vertexBuffer.getBuffer(); }
    const VkBuffer getIndexBuffer() const { return m_indexBuffer ? m_indexBuffer->getBuffer() : VK_NULL_HANDLE; }
    const size_t getIndexTypeSize() const { return m_indexTypeSize; }
    const uint32_t getIndexBufferLength() const { return m_indexBufferLength; }

private:

    void placeContentInGpuMemory(const VulkanBuffer& buffer, const void *data, const VkDeviceSize size);

    const PhysicalDevice& m_physicalDevice;
    const VkDevice& m_device;
    const VkQueue& m_queue;
    const VkCommandPool& m_commandPool;
    VulkanBuffer m_vertexBuffer;
    std::optional<VulkanBuffer> m_indexBuffer;
    const size_t m_indexTypeSize;
    const uint32_t m_indexBufferLength;
};
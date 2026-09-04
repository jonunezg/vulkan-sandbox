#include "VulkanVertexBuffer.h"

VulkanVertexBuffer::VulkanVertexBuffer(
    const PhysicalDevice& physicalDevice,
    const VkDevice& device,
    const VkQueue& queue,
    const VkCommandPool& commandPool,
    const std::vector<Geometry::Vertex>& vertices,
    const std::optional<std::vector<uint16_t>> indices) :
m_physicalDevice { physicalDevice },
m_device { device },
m_queue { queue },
m_commandPool { commandPool },
m_indexTypeSize { indices ? sizeof((*indices)[0]) : 0 },
m_indexBufferLength { indices ? static_cast<uint32_t>(sizeof((*indices)[0]) * indices->size()) : 0 },
m_vertexBuffer
{
    m_physicalDevice,
    m_device,
    sizeof(vertices[0]) * vertices.size(),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
}
{
    const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    placeContentInGpuMemory(m_vertexBuffer, vertices.data(), bufferSize);

    if (indices)
    {
        m_indexBuffer.emplace(
            m_physicalDevice,
            m_device,
            m_indexBufferLength,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        placeContentInGpuMemory(*m_indexBuffer, indices->data(), m_indexBufferLength);
    }
    
    LOG_ENGINE_INFO("Vulkan vertex buffer created");
}

void VulkanVertexBuffer::placeContentInGpuMemory(const VulkanBuffer& buffer, const void *data, const VkDeviceSize size)
{
    VulkanTransferCommandBuffer transferCommandBuffer { m_device, m_commandPool };

    VulkanBuffer stagingBuffer
    {
        m_physicalDevice,
        m_device,
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };
    
    void *mem;
    vkMapMemory(m_device, stagingBuffer.getBufferMemory(), 0, size, 0, &mem);
    memcpy(mem, data, static_cast<size_t>(size));
    vkUnmapMemory(m_device, stagingBuffer.getBufferMemory());

    transferCommandBuffer.transferBufferContent(m_queue, size, stagingBuffer.getBuffer(), buffer.getBuffer());
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
    LOG_ENGINE_INFO("Vulkan vertex buffer destroyed");
}
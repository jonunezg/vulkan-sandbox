#include "VulkanVertexBuffer.h"

VulkanVertexBuffer::VulkanVertexBuffer(
    const PhysicalDevice& physicalDevice,
    const VkDevice& device,
    const VkQueue& queue,
    const VkCommandPool& commandPool,
    const std::vector<Geometry::Vertex>& vertices) :
m_physicalDevice { physicalDevice },
m_device { device },
m_vertexBuffer
{
    m_physicalDevice,
    m_device,
    sizeof(vertices[0]) * vertices.size(),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
},
m_transferCommandBuffer
{
    device,
    commandPool
}
{
    const VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VulkanBuffer stagingBuffer
    {
        m_physicalDevice,
        m_device,
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };
    
    void *data;
    vkMapMemory(m_device, stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_device, stagingBuffer.getBufferMemory());

    m_transferCommandBuffer.transferBufferContent(queue, bufferSize, stagingBuffer.getBuffer(), m_vertexBuffer.getBuffer());
    
    LOG_ENGINE_INFO("Vulkan vertex buffer created");
}

VulkanVertexBuffer::~VulkanVertexBuffer()
{
    LOG_ENGINE_INFO("Vulkan vertex buffer destroyed");
}
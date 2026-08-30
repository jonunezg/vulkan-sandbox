#include "VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(const PhysicalDevice& physicalDevice, const VkDevice& device, const std::vector<Geometry::Vertex>& vertices) :
m_physicalDevice { physicalDevice },
m_device { device }
{
    const VkBufferCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = sizeof(vertices[0]) * vertices.size(),
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = nullptr,
    };

    VK_THROW_IF_FAILED(vkCreateBuffer(m_device, &createInfo, nullptr, &m_vertexBuffer));

    
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memoryRequirements);
    
    const VkMemoryAllocateInfo allocateInfo
    {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .pNext = nullptr,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = m_physicalDevice.findMemoryType(memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
    };
        
    VK_THROW_IF_FAILED(vkAllocateMemory(m_device, &allocateInfo, nullptr, &m_vertexBufferMemory));
    
    vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexBufferMemory, 0);
    
    void *data;
    vkMapMemory(m_device, m_vertexBufferMemory, 0, createInfo.size, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(createInfo.size));
    vkUnmapMemory(m_device, m_vertexBufferMemory);
    
    LOG_ENGINE_INFO("Vulkan vertex buffer created: " << m_vertexBuffer);
}

VulkanBuffer::~VulkanBuffer()
{
    if (m_vertexBuffer && m_vertexBufferMemory)
    {
        vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
        vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);

        LOG_ENGINE_INFO("Vulkan vertex buffer destroyed: " << m_vertexBuffer);
    }
}
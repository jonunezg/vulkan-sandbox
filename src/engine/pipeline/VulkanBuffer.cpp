#include "VulkanBuffer.h"

VulkanBuffer::VulkanBuffer(
        const PhysicalDevice& physicalDevice,
        const VkDevice& device,
        const VkDeviceSize size,
        const VkBufferUsageFlags usage,
        const VkMemoryPropertyFlags properties) :
m_physicalDevice { physicalDevice },
m_device { device }
{
    const VkBufferCreateInfo createInfo
    {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .size = size,
        .usage = usage,
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

    LOG_ENGINE_INFO("Vulkan buffer created: " << m_vertexBuffer);
}

VulkanBuffer::~VulkanBuffer()
{
    if (m_vertexBuffer && m_vertexBufferMemory)
    {
        vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
        vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);

        LOG_ENGINE_INFO("Vulkan buffer destroyed: " << m_vertexBuffer);
    }
}
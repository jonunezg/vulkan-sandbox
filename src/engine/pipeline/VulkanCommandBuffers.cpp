#include "VulkanCommandBuffers.h"

VulkanCommandBuffers::VulkanCommandBuffers(const VkDevice& device, const VkCommandPool& commandPool, uint32_t commandBufferCount) :
m_device { device }
{
    VkCommandBufferAllocateInfo bufferAllocateInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = commandBufferCount,
    };

    m_commandBuffers.resize(MAX_CONCURRENT_IMAGES);

    VK_THROW_IF_FAILED(vkAllocateCommandBuffers(m_device, &bufferAllocateInfo, m_commandBuffers.data()));

    for(const auto& commandBuffer : m_commandBuffers)
    {
        LOG_ENGINE_INFO("Vulkan command buffer created: " << commandBuffer);
    }
}
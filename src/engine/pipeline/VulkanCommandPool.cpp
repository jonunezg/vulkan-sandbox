#include "VulkanCommandPool.h"

VulkanCommandPool::VulkanCommandPool(const VkDevice& logicalDevice, const PhysicalDevice& physicalDevice) :
m_device { logicalDevice }
{
    VkCommandPoolCreateInfo commandPoolCreateInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = physicalDevice.graphicQueueIndex.value(),
    };

    VK_THROW_IF_FAILED(vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool));

    LOG_ENGINE_INFO("Vulkan command pool created: " << m_commandPool);
}

VulkanCommandPool::~VulkanCommandPool()
{
    if (m_commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);

        LOG_ENGINE_INFO("Vulkan command pool destroyed: " << m_commandPool);
    }
}

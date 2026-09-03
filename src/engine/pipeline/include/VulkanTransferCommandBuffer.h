#pragma once

#include "Utilities.h"
#include "VulkanCommandBuffers.h"
#include "VulkanIncludes.h"
#include "VulkanLogger.h"

class VulkanTransferCommandBuffer : VulkanCommandBuffers
{
public:
    VulkanTransferCommandBuffer(const VkDevice& device, const VkCommandPool& commandPool) :
    VulkanCommandBuffers { device, commandPool, 1 }
    {}

    void transferBufferContent(const VkQueue& queue, const VkDeviceSize& bufferSize, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer)
    {
        VkCommandBufferBeginInfo beginInfo
        {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = nullptr,
        };

        VK_THROW_IF_FAILED(vkBeginCommandBuffer(m_commandBuffers[0], &beginInfo));

        const VkBufferCopy bufferCopy
        {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = bufferSize,
        };

        vkCmdCopyBuffer(m_commandBuffers[0], srcBuffer, dstBuffer, 1, &bufferCopy);

        VK_THROW_IF_FAILED(vkEndCommandBuffer(m_commandBuffers[0]));

        const VkSubmitInfo submitInfo
        {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &m_commandBuffers[0],
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
        };

        vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(queue);
    }
};
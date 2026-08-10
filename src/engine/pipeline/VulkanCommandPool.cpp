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

    createCommandBuffer();
}

VulkanCommandPool::~VulkanCommandPool()
{
    if (m_commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_device, m_commandPool, nullptr);

        LOG_ENGINE_INFO("Vulkan command pool destroyed: " << m_commandPool);
    }
}

void VulkanCommandPool::createCommandBuffer()
{
    VkCommandBufferAllocateInfo bufferAllocateInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = nullptr,
        .commandPool = m_commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VK_THROW_IF_FAILED(vkAllocateCommandBuffers(m_device, &bufferAllocateInfo, &m_commandBuffer));

    LOG_ENGINE_INFO("Vulkan command buffer created: " << m_commandBuffer);
}

void VulkanCommandPool::recordCommandBuffer(
    uint32_t imageIndex,
    VkRenderPass renderPass,
    const std::vector<VkFramebuffer>& framebuffers,
    VkExtent2D swapchainExtent,
    VkPipeline pipeline)
{
    VkCommandBufferBeginInfo beginInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };

    VK_THROW_IF_FAILED(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));

    VkClearValue clearColor = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
    VkRenderPassBeginInfo renderPassInfo
    {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = renderPass,
        .framebuffer = framebuffers[imageIndex],
        .renderArea = {{0, 0}, swapchainExtent},
        .clearValueCount = 1,
        .pClearValues = &clearColor,
    };

    vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    const VkViewport viewport
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(swapchainExtent.width),
        .height = static_cast<float>(swapchainExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    const VkRect2D scissor
    {
        .offset = { 0, 0},
        .extent = swapchainExtent,
    };

    vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
    //Future: read about instanced rendering

    vkCmdEndRenderPass(m_commandBuffer);

    VK_THROW_IF_FAILED(vkEndCommandBuffer(m_commandBuffer));
}
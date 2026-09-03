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
        .commandBufferCount = 2,
    };

    m_commandBuffers.resize(MAX_CONCURRENT_IMAGES);

    VK_THROW_IF_FAILED(vkAllocateCommandBuffers(m_device, &bufferAllocateInfo, m_commandBuffers.data()));

    for(const auto& commandBuffer : m_commandBuffers)
    {
        LOG_ENGINE_INFO("Vulkan command buffer created: " << commandBuffer);
    }
}

//!TODO: Create a CommandBuffer base class and specialization for each type of command buffer
void VulkanCommandPool::recordCommandBuffer(
    size_t frameIndex,
    uint32_t imageIndex,
    VkRenderPass renderPass,
    const std::vector<VkFramebuffer>& framebuffers,
    VkExtent2D swapchainExtent,
    VkPipeline pipeline,
    const VulkanVertexBuffer& vertexBuffer)
{
    VkCommandBufferBeginInfo beginInfo
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0,
        .pInheritanceInfo = nullptr,
    };

    VK_THROW_IF_FAILED(vkBeginCommandBuffer(m_commandBuffers[frameIndex], &beginInfo));

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

    vkCmdBeginRenderPass(m_commandBuffers[frameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(m_commandBuffers[frameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

    VkBuffer vertexBuffers[] = { vertexBuffer.getBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(m_commandBuffers[frameIndex], 0, 1, vertexBuffers, offsets);

    const VkViewport viewport
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(swapchainExtent.width),
        .height = static_cast<float>(swapchainExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    vkCmdSetViewport(m_commandBuffers[frameIndex], 0, 1, &viewport);

    const VkRect2D scissor
    {
        .offset = { 0, 0},
        .extent = swapchainExtent,
    };

    vkCmdSetScissor(m_commandBuffers[frameIndex], 0, 1, &scissor);

    vkCmdDraw(m_commandBuffers[frameIndex], 3, 1, 0, 0);
    //Future: read about instanced rendering

    vkCmdEndRenderPass(m_commandBuffers[frameIndex]);

    VK_THROW_IF_FAILED(vkEndCommandBuffer(m_commandBuffers[frameIndex]));
}
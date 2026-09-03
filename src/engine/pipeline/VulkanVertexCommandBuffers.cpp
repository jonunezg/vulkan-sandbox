#include "VulkanVertexCommandBuffers.h"

void VulkanVertexCommandBuffers::recordCommandBuffer(
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
#pragma once

#include <array>

#include "glm/glm.hpp"

#include "Utilities.h"
#include "VulkanIncludes.h"

namespace Geometry
{
    typedef struct _Vertex
    {
        glm::vec2 position;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription()
        {
            return VkVertexInputBindingDescription
            {
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
            };
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            return std::array<VkVertexInputAttributeDescription, 2>
            {
                VkVertexInputAttributeDescription
                {
                    .location = 0,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32_SFLOAT,
                    .offset = offsetof(Vertex, position),
                },
                VkVertexInputAttributeDescription
                {
                    .location = 1,
                    .binding = 0,
                    .format = VK_FORMAT_R32G32B32_SFLOAT,
                    .offset = offsetof(Vertex, color),
                }
            };
        }
    } Vertex;
}

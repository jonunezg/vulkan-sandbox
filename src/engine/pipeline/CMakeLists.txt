cmake_minimum_required(VERSION 3.15)

add_library(vulkan_pipeline STATIC
    ./VulkanPipelineManager.cpp
    ./VulkanShaderModule.cpp
)

target_link_libraries(vulkan_pipeline vulkan_device)

target_include_directories(vulkan_pipeline PUBLIC include)

target_link_libraries(vulkan_pipeline glm::glm)
target_link_libraries(vulkan_pipeline ${glfw3_LIBRARIES})
target_link_libraries(vulkan_pipeline vulkan-headers::vulkan-headers)
target_link_libraries(vulkan_pipeline ${VulkanLoader_LIBRARIES})

set_property(TARGET vulkan_pipeline PROPERTY CXX_STANDARD 20)
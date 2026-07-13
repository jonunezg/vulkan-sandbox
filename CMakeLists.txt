cmake_minimum_required(VERSION 3.15)
project(vulkan CXX)

find_package(glm REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(VulkanLoader REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/engine/VulkanExtensions.cpp
    src/engine/VulkanImageView.cpp
    src/engine/VulkanInstance.cpp
    src/engine/VulkanLayers.cpp
    src/engine/VulkanLogger.cpp
    src/engine/VulkanManager.cpp
    src/engine/VulkanLogicalDevice.cpp
    src/engine/VulkanPhysicalDevice.cpp
    src/engine/VulkanSurface.cpp
    src/engine/VulkanSwapchain.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${glfw3_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} ${glfw3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} vulkan-headers::vulkan-headers)
target_link_libraries(${PROJECT_NAME} ${VulkanLoader_LIBRARIES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Move Vulkan debug config file
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/engine/vk_layer_settings.txt
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Compile vertex shader, consider creating a runtime compilation and cache system in the future
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND glslc ${CMAKE_SOURCE_DIR}/src/shaders/triangle.vert
    -o $<TARGET_FILE_DIR:${PROJECT_NAME}>/triangle_vert.spv
)

# Compile fragment shader
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND glslc ${CMAKE_SOURCE_DIR}/src/shaders/triangle.frag
    -o $<TARGET_FILE_DIR:${PROJECT_NAME}>/triangle_frag.spv
)
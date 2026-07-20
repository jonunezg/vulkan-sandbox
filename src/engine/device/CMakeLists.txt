cmake_minimum_required(VERSION 3.15)

add_library(vulkan_device STATIC
    ./VulkanDeviceManager.cpp
    ./VulkanExtensions.cpp
    ./VulkanImageView.cpp
    ./VulkanInstance.cpp
    ./VulkanLayers.cpp
    ./VulkanLogger.cpp
    ./VulkanLogicalDevice.cpp
    ./VulkanPhysicalDevice.cpp
    ./VulkanSurface.cpp
    ./VulkanSwapchain.cpp
)

target_include_directories(vulkan_device PUBLIC include)
target_include_directories(vulkan_device PUBLIC ${glfw3_INCLUDE_DIRS})
target_include_directories(vulkan_device PUBLIC ../utilities)

target_link_libraries(vulkan_device glm::glm)
target_link_libraries(vulkan_device ${glfw3_LIBRARIES})
target_link_libraries(vulkan_device vulkan-headers::vulkan-headers)
target_link_libraries(vulkan_device ${VulkanLoader_LIBRARIES})

set_property(TARGET vulkan_device PROPERTY CXX_STANDARD 20)
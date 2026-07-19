cmake_minimum_required(VERSION 3.15)
project(vulkan CXX)

find_package(glm REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(VulkanHeaders REQUIRED)
find_package(VulkanLoader REQUIRED)

add_subdirectory(src)
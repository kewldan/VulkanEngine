cmake_minimum_required(VERSION 3.25)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(ZLIB REQUIRED)
find_path(VULKAN_HPP_INCLUDE_DIRS "vulkan/vulkan.hpp")

include_directories(
        include
        engine/include
)

file(GLOB_RECURSE PROJECT_SRCS
        src/*.cpp
        include/*.h
        )

file(GLOB_RECURSE ENGINE_SRCS
        engine/src/*.cpp
        engine/include/*.h
        )

add_executable(VulkanEngine ${PROJECT_SRCS} ${ENGINE_SRCS})

target_include_directories(${PROJECT_NAME} PRIVATE ${VULKAN_HPP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw plog::plog Vulkan::Vulkan ZLIB::ZLIB)

cmake_minimum_required(VERSION 3.25)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DMAX_FRAMES_IN_FLIGHT=2)

find_package(glfw3 CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(ZLIB REQUIRED)
find_package(VulkanHeaders CONFIG)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)

include_directories(
        include
        engine/include
        lib
)

file(GLOB_RECURSE PROJECT_SRCS
        src/*.cpp
        include/*.h
        )

file(GLOB_RECURSE ENGINE_SRCS
        engine/src/*.cpp
        engine/include/*.h
        )

add_executable(${PROJECT_NAME} ${PROJECT_SRCS} ${ENGINE_SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw plog::plog Vulkan::Vulkan ZLIB::ZLIB Vulkan::Headers GPUOpen::VulkanMemoryAllocator imgui::imgui glm::glm ${BULLET_LIBRARIES})

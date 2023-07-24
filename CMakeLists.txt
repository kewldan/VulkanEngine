cmake_minimum_required(VERSION 3.25)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DMAX_FRAMES_IN_FLIGHT=2)

find_package(glfw3 CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(unofficial-omniverse-physx-sdk CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

find_package(Vulkan)
find_package(VulkanMemoryAllocator CONFIG REQUIRED)
find_package(VulkanHeaders CONFIG)

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

target_link_libraries(${PROJECT_NAME} PRIVATE glfw plog::plog ZLIB::ZLIB GPUOpen::VulkanMemoryAllocator glm::glm unofficial::omniverse-physx-sdk::sdk imgui::imgui Vulkan::Headers Vulkan::Vulkan)

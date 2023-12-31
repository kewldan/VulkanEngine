cmake_minimum_required(VERSION 3.25)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DMAX_FRAMES_IN_FLIGHT=2)

find_package(glfw3 CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Bullet CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(meshoptimizer CONFIG REQUIRED)
find_package(base64 CONFIG REQUIRED)

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
)

file(GLOB_RECURSE ENGINE_SRCS
        engine/src/*.cpp
)

add_executable(VulkanEngine ${PROJECT_SRCS} ${ENGINE_SRCS})
set_property(TARGET VulkanEngine PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(VulkanEngine PRIVATE glfw plog::plog ZLIB::ZLIB GPUOpen::VulkanMemoryAllocator glm::glm imgui::imgui Vulkan::Headers Vulkan::Vulkan ${BULLET_LIBRARIES} meshoptimizer::meshoptimizer aklomp::base64)
target_include_directories(VulkanEngine PRIVATE ${Stb_INCLUDE_DIR})

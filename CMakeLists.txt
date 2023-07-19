cmake_minimum_required(VERSION 3.25)
project(VulkanEngine)

set(CMAKE_CXX_STANDARD 17)

set(VCPKG_LIBRARIES_PATH ${VCPKG_INSTALLED_DIR}/x64-windows-static)
set(VCPKG_LIBRARIES_PATH_MD ${VCPKG_INSTALLED_DIR}/x64-windows-static-md)

include_directories(
        ${VCPKG_LIBRARIES_PATH}/include
        ${VCPKG_LIBRARIES_PATH_MD}/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    link_directories("${VCPKG_LIBRARIES_PATH}/lib")
    link_directories("${VCPKG_LIBRARIES_PATH_MD}/lib")
else ()
    link_directories("${VCPKG_LIBRARIES_PATH}/debug/lib")
    link_directories("${VCPKG_LIBRARIES_PATH_MD}/debug/lib")
    set(_D d)
    set(_DL _Debug)
endif ()


include_directories(
        include
        engine/include
        C:/VulkanSDK/1.3.250.1/Include
)

link_directories(
        C:/VulkanSDK/1.3.250.1/Lib
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

target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1 zlib${_D} imgui${_D} freetype${_D} bz2${_D} libpng16${_D} brotlicommon-static brotlidec-static brotlienc-static)
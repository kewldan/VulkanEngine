#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace Engine {
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription();

        static VkVertexInputAttributeDescription *getAttributeDescriptions(int *);
    };
}

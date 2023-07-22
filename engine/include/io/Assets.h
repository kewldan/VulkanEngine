#pragma once

#include "Mesh.h"

namespace Engine {
    class Assets {
    public:
        static Mesh *loadMeshes(const char *filename, int *count);

        static VkShaderModule loadShader(VkDevice device, const char *filename);
    };
}

#pragma once

#include "graphics/Mesh.h"
#include "common/GameObject.h"
#include <vector>
#include <functional>
#include <future>

namespace Engine {
    struct AssetLoadLayout {
        std::function<void()> load{};
        std::function<void()> upload{};
        std::thread loadingThread{};

        AssetLoadLayout();
    };

    class AssetLoader {
    private:
        static std::vector<AssetLoadLayout *> queue;
    public:
        static void loadMeshes(const char *filename, Mesh **meshes, int *count);

        static VkShaderModule loadShader(VkDevice device, const char *filename);

        static void loadGameObject(VmaAllocator allocator, GameObject &gameObject, const char *filename);

        static void asyncLoad();
    };
}

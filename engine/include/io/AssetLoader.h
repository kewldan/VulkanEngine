#pragma once

#include "graphics/Mesh.h"
#include "common/GameObject.h"
#include <vector>
#include <functional>
#include <future>

namespace Engine {
    struct AssetTask {
        std::function<void()> load{};
    };

    struct TaskQueue {
        std::vector<AssetTask *> tasks;
        std::thread worker;

        void execute();
    };

    template<int Count>
    struct TaskSolver {
        TaskQueue queues[Count];
        int index = 0;

        void addTask(AssetTask *task);

        void execute();
    };

    template<int Count>
    void TaskSolver<Count>::execute() {
        for (int i = 0; i < Count; i++) {
            queues[i].execute();
        }

        for (int i = 0; i < Count; i++) {
            queues[i].worker.join();
        }
    }

    template<int Count>
    void TaskSolver<Count>::addTask(AssetTask *task) {
        queues[index].tasks.push_back(task);
        if (++index == Count) {
            index = 0;
        }
    }

    class AssetLoader {
    private:
        static TaskSolver<4> solver;

        static void loadMeshes(const char *filename, Mesh **meshes, int *count);

    public:
        static void loadShader(VkDevice device, VkShaderModule *shaderModule, const char *filename);

        static void loadGameObject(VmaAllocator allocator, GameObject &gameObject, const char *filename);

        static void asyncLoad();
    };
}

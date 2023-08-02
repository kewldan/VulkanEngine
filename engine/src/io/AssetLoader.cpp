#include "io/AssetLoader.h"
#include "io/Filesystem.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    void AssetLoader::asyncLoad() {
        solver.execute();
    }

    TaskSolver<4> AssetLoader::solver{};

    void AssetLoader::loadAsset(Asset &asset) {
        auto *layout = new AssetTask;
        layout->load = [&asset]() {
            asset.load();
        };
        solver.addTask(layout);
    }

    void TaskQueue::execute() {
        worker = std::thread([this]() {
            for (auto *task: tasks) {
                task->load();
            }
        });
    }
}
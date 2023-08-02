#include <cassert>
#include "io/AssetLoader.h"
#include "io/Filesystem.h"
#include "graphics/Mesh.h"
#include "OBJ_Loader.h"
#include "graphics/VulkanContext.h"

namespace Engine {
    void AssetLoader::loadMeshes(const char *filename, Mesh **meshes, size_t *count) {
        assert(filename != nullptr);
        assert(count != nullptr);
        assert(meshes != nullptr);

        objl::Loader Loader;
        bool loadout = Loader.LoadFile(filename);

        *count = Loader.LoadedMeshes.size();
        *meshes = new Mesh[*count];

        if (loadout) {
            int i = 0;
            for (const auto &curMesh: Loader.LoadedMeshes) {
                auto *vertices = new Vertex[curMesh.Vertices.size()];
                int j = 0;
                for (const auto &Vert: curMesh.Vertices) {
                    vertices[j].pos = glm::vec3(Vert.Position.X, Vert.Position.Y, Vert.Position.Z);
                    vertices[j].normal = glm::vec3(Vert.Normal.X, Vert.Normal.Y, Vert.Normal.Z);
                    vertices[j].uv = glm::vec2(Vert.TextureCoordinate.X, Vert.TextureCoordinate.Y);
                    j++;
                }
                auto *indices = new uint16_t[curMesh.Indices.size()];
                for (int k = 0; k < curMesh.Indices.size(); k++) {
                    indices[k] = curMesh.Indices[k];
                }
                *meshes[i] = Mesh(vertices, indices, curMesh.Indices.size(), curMesh.Vertices.size());
                i++;
            }
        }
    }

    void AssetLoader::loadShader(VkShaderModule *shaderModule, const char *filename) {
        assert(filename != nullptr);
        assert(shaderModule != nullptr);

        auto *task = new AssetTask;
        task->load = [filename, shaderModule]() { // Filename is not reference because filename is pointer and should be copy
            size_t length;
            auto code = Filesystem::readFile(filename, &length);

            VkShaderModuleCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            createInfo.codeSize = length;
            createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

            if (vkCreateShaderModule(VulkanContext::device, &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
                throw std::runtime_error("failed to create shader module!");
            }
        };
        solver.addTask(task);
    }

    void AssetLoader::loadGameObject(GameObject *gameObject, const char *filename) {
        assert(filename != nullptr);

        auto *layout = new AssetTask;
        layout->load = [filename, gameObject]() { // Filename is not reference because filename is pointer and should be copy
            loadMeshes(filename, &gameObject->meshes, &gameObject->meshCount);
            gameObject->upload();
        };
        solver.addTask(layout);
    }

    void AssetLoader::asyncLoad() {
        solver.execute();
    }

    TaskSolver<4> AssetLoader::solver{};

    void TaskQueue::execute() {
        worker = std::thread([this]() {
            for (auto *task: tasks) {
                task->load();
            }
        });
    }
}
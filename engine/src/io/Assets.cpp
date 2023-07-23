#include <cassert>
#include "io/Assets.h"
#include "io/Filesystem.h"
#include "graphics/Mesh.h"
#include "OBJ_Loader.h"

namespace Engine {

    Mesh *Assets::loadMeshes(const char *filename, int *count) {
        assert(filename != nullptr);
        assert(count != nullptr);

        objl::Loader Loader;
        bool loadout = Loader.LoadFile(filename);

        *count = Loader.LoadedMeshes.size();
        Mesh *meshes = new Mesh[*count];

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
                meshes[i] = Mesh(vertices, indices, curMesh.Indices.size(), curMesh.Vertices.size());
                i++;
            }
        }

        return meshes;
    }

    VkShaderModule Assets::loadShader(VkDevice device, const char *filename) {
        assert(device != VK_NULL_HANDLE);
        assert(filename != nullptr);

        size_t length;
        auto code = Filesystem::readFile(filename, &length);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = length;
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code);

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }
}
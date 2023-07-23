#include "common/GameObject.h"

namespace Engine {
    GameObject::GameObject() = default;

    void GameObject::upload(VmaAllocator allocator) const {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].upload(allocator);
        }
    }

    void GameObject::cleanup(VmaAllocator allocator) const {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].cleanup(allocator);
        }
    }
}
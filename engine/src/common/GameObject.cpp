#include "common/GameObject.h"

namespace Engine {
    GameObject::GameObject() = default;

    void GameObject::upload() const {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].upload();
        }
    }

    void GameObject::destroy() {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].destroy();
        }
    }
}
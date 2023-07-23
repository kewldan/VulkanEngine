#include "common/GameObject.h"

namespace Engine {
    GameObject::GameObject() = default;

    void GameObject::upload(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool,
                            VkQueue graphicsQueue) const {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].upload(physicalDevice, device, commandPool, graphicsQueue);
        }
    }

    void GameObject::cleanup(VkDevice device) const {
        for (int i = 0; i < meshCount; i++) {
            meshes[i].cleanup(device);
        }
    }
}
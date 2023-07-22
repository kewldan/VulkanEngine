#pragma once

#include "Application.h"
#include "Mesh.h"
#include "vulkan/UniformBuffer.h"
#include "Camera3D.h"
#include "GameObject.h"
#include <vector>

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class Game : public Engine::Application {
private:
    std::unique_ptr<Engine::GameObject> gameObject;
    VkPipeline graphicsPipeline{};
    std::unique_ptr<Engine::UniformBuffer<UniformBufferObject>> uniformBuffer;
    std::unique_ptr<Engine::Camera3D> camera;

    std::vector<VkDescriptorSet> descriptorSets;

    void createGraphicsPipeline();

    void createDescriptorSets();

public:
    void createWindow() override;

    void init() override;

    void render(VkCommandBuffer commandBuffer) override;

    void update() override;

    void cleanup() override;

    void gui() override;
};

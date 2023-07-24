#pragma once

#include "common/Application.h"
#include "graphics/UniformBuffer.h"
#include "camera/Camera3D.h"
#include "graphics/VulkanHelper.h"
#include <vector>
#include "physics/GameWorld.h"

struct Uniform_CameraData {
    glm::mat4 view;
    glm::mat4 proj;
};

struct Uniform_ModelData {
    glm::mat4 model;
};

class Game : public Engine::Application {
private:
    Engine::GameObject cubeGameObject, planeGameObject;
    VkPipelineLayout pipelineLayout{};
    VkPipeline graphicsPipeline{};
    Engine::UniformBuffer<Uniform_CameraData> uniformCamera;
    Engine::Camera3D camera;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    Engine::GameWorld world;

    void createGraphicsPipeline();

    void createPipelineLayout();

public:
    void createWindow() override;

    void loadAssets() override;

    void init() override;

    void render(VkCommandBuffer commandBuffer) override;

    void update() override;

    void cleanup() override;

    void gui() override;
};

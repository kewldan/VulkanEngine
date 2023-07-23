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
    std::unique_ptr<Engine::GameObject> cubeGameObject, planeGameObject;
    VkPipelineLayout pipelineLayout{};
    VkPipeline graphicsPipeline{};
    std::unique_ptr<Engine::UniformBuffer<Uniform_CameraData>> uniformCamera;
    std::unique_ptr<Engine::Camera3D> camera;
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    std::unique_ptr<Engine::GameWorld> world;

    void createGraphicsPipeline();

    void createPipelineLayout();

public:
    void createWindow() override;

    void init() override;

    void render(VkCommandBuffer commandBuffer) override;

    void update() override;

    void cleanup() override;

    void gui() override;
};

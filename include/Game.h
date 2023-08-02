#pragma once

#include "common/Application.h"
#include "camera/Camera3D.h"
#include "graphics/VulkanHelper.h"
#include <vector>
#include "physics/GameWorld.h"
#include "graphics/RenderPipelineLayout.h"
#include "LitPipeline.h"

struct Uniform_CameraData {
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec4 position;
};

struct Uniform_ModelData {
    glm::vec4 color;
    glm::mat4 model;
};

class Game : public Engine::Application {
private:
    Engine::GameObject *cubeGameObject, *planeGameObject;
    Engine::RenderPipelineLayout pipelineLayout{};
    LitPipeline graphicsPipeline;
    Engine::UniformBuffer<Uniform_CameraData> uniformCamera;
    Engine::PushBuffer<Uniform_ModelData> uniformModel;
    Engine::Camera3D camera;
    Engine::GameWorld world;
public:
    void createWindow() override;

    void loadAssets() override;

    void preInit() override;

    void init() override;

    void render(VkCommandBuffer commandBuffer) override;

    void update() override;

    void destroy() override;

    void gui() override;
};

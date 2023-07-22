#pragma once

#include "Application.h"
#include "Mesh.h"

class Game : public Engine::Application {
private:
    Engine::Mesh *mesh{};
    VkPipeline graphicsPipeline{};
public:
    void createWindow() override;

    void init() override;

    void render(VkCommandBuffer commandBuffer, VkDescriptorSet *descriptorSet) override;

    void update() override;

    void cleanup() override;

    void gui() override;
};

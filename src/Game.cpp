#include "Game.h"
#include "imgui.h"

void Game::init() {
    mesh = new Engine::Mesh({
                                    new Engine::Vertex[]{
                                            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                            {{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},
                                            {{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}},
                                            {{-0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}}
                                    },
                                    new uint16_t[]{0, 1, 2, 2, 3, 0},
                                    6,
                                    4
                            });
    mesh->upload(context.physicalDevice, context.device, context.commandPool, context.graphicsQueue);
}

void Game::update() {

}

void Game::cleanup() {
    mesh->cleanup(context.device);
    window->cleanup();
}

void Game::createWindow() {
    window = new Engine::Window("Vulkan game", 800, 600);
}

void Game::render(VkCommandBuffer commandBuffer, VkDescriptorSet *descriptorSet) {
    VkBuffer vertexBuffers[] = {mesh->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer, 0,
                         VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            context.pipelineLayout, 0, 1,
                            descriptorSet, 0, nullptr);
    vkCmdDrawIndexed(commandBuffer, mesh->indexCount, 1, 0, 0, 0);
}

void Game::gui() {
    ImGui::ShowDemoWindow();
}

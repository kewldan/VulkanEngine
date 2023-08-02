#include "Game.h"

#include "io/AssetLoader.h"
#include "graphics/BufferHandler.h"
#include "plog/Log.h"
#include "common/Engine.h"
#include "graphics/VulkanContext.h"
#include "graphics/RenderPipeline.h"
#include "LitPipeline.h"

#include <stdexcept>
#include <algorithm>
#include <thread>
#include <imgui.h>

void Game::init() {
    Engine::InputManager::setRawInput(window->getHandle(), true);
    Engine::InputManager::setLockCursor(window->getHandle(), true);
    Engine::InputManager::registerCallbacks(window->getHandle());

    Engine::InputManager::keyPressListeners.emplace_back([this](int key) {
        if (key == GLFW_KEY_ESCAPE) {
            Engine::InputManager::setLockCursor(window->getHandle(), !Engine::InputManager::lockCursor);
        }
    });

    Engine::InputManager::keyPressListeners.emplace_back([](int key) {
        if (key == GLFW_KEY_F) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    world.init();

    planeGameObject.scale = glm::vec3(30.f, 1.f, 30.f);

    uniformCamera.init(VK_SHADER_STAGE_VERTEX_BIT, 0);

    descriptorSetLayouts.push_back(uniformCamera.layout);

    camera.position = glm::vec3(2.f, 2.f, 2.f);
    camera.rotation = glm::vec3(0.6f, -0.8f, 0.f);

    createPipelineLayout();
    createGraphicsPipeline();
}

void Game::update() {
    glm::vec2 delta = Engine::InputManager::getCursorDelta(window->getHandle());
    camera.rotation.x += delta.y * 0.004f;
    camera.rotation.y += delta.x * 0.004f;
    camera.rotation.x = std::clamp(camera.rotation.x, -1.5f, 1.5f);

    glm::vec3 vel(0.f);
    float speed = 3.5f;
    float crouch = ImGui::GetIO().DeltaTime;
    if (glfwGetKey(window->getHandle(), GLFW_KEY_W)) {
        vel.x -= std::cos(camera.rotation.y + 1.57f) * speed * crouch;
        vel.y -= std::sin(camera.rotation.x) * speed * crouch;
        vel.z -= std::sin(camera.rotation.y + 1.57f) * speed * crouch;
    } else if (glfwGetKey(window->getHandle(), GLFW_KEY_S)) {
        vel.x += std::cos(camera.rotation.y + 1.57f) * speed * crouch;
        vel.y += std::sin(camera.rotation.x) * speed * crouch;
        vel.z += std::sin(camera.rotation.y + 1.57f) * speed * crouch;
    }

    if (glfwGetKey(window->getHandle(), GLFW_KEY_A)) {
        vel.x -= std::cos(camera.rotation.y) * speed * crouch;
        vel.z -= std::sin(camera.rotation.y) * speed * crouch;
    } else if (glfwGetKey(window->getHandle(), GLFW_KEY_D)) {
        vel.x += std::cos(camera.rotation.y) * speed * crouch;
        vel.z += std::sin(camera.rotation.y) * speed * crouch;
    }
    camera.position += vel;

    camera.update((float) Engine::VulkanContext::swapChainExtent.width,
                  (float) Engine::VulkanContext::swapChainExtent.height);

    uniformCamera.view = camera.getView();
    uniformCamera.proj = camera.getProjection();
    uniformCamera.proj[1][1] *= -1;
    uniformCamera.position = glm::vec4(camera.position, 0.f);

    uniformCamera.upload();
}

void Game::destroy() {
    world.destroy();
    vkDestroyPipelineLayout(Engine::VulkanContext::device, pipelineLayout, nullptr);
    vkDestroyPipeline(Engine::VulkanContext::device, graphicsPipeline, nullptr);

    for (VkDescriptorSetLayout layout: descriptorSetLayouts) {
        vkDestroyDescriptorSetLayout(Engine::VulkanContext::device, layout, nullptr);
    }

    uniformCamera.destroy();
    cubeGameObject.destroy();
    planeGameObject.destroy();
    window->destroy();
}

void Game::createWindow() {
    window = std::make_unique<Engine::Window>("Vulkan game", 1280, 720);
}

void Game::render(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) Engine::VulkanContext::swapChainExtent.width;
    viewport.height = (float) Engine::VulkanContext::swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = Engine::VulkanContext::swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    static Uniform_ModelData data{};

    data.color = glm::vec4(1, 1, 0, 1);
    data.model = cubeGameObject.getModel();
    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Uniform_ModelData),
                       &data);
    for (int i = 0; i < cubeGameObject.meshCount; i++) {
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &cubeGameObject.meshes[i].vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, cubeGameObject.meshes[i].indexBuffer.buffer, 0,
                             VK_INDEX_TYPE_UINT16);
        VkDescriptorSet sets[] = {
                uniformCamera.descriptorSets[Engine::VulkanContext::currentFrame]
        };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout, 0, 1,
                                sets, 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, cubeGameObject.meshes[i].indexCount, 1, 0, 0, 0);
    }

    data.color = glm::vec4(1, 0, 0, 1);
    data.model = planeGameObject.getModel();
    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Uniform_ModelData),
                       &data);
    for (int i = 0; i < planeGameObject.meshCount; i++) {
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &planeGameObject.meshes[i].vertexBuffer.buffer, offsets);
        vkCmdBindIndexBuffer(commandBuffer, planeGameObject.meshes[i].indexBuffer.buffer, 0,
                             VK_INDEX_TYPE_UINT16);

        VkDescriptorSet sets[] = {
                uniformCamera.descriptorSets[Engine::VulkanContext::currentFrame]
        };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout, 0, 1,
                                sets, 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, planeGameObject.meshes[i].indexCount, 1, 0, 0, 0);
    }
}

void Game::gui() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::SetNextWindowBgAlpha(0.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
    if (ImGui::Begin("##PerfomanceOverlay", nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
        ImGui::PlotLines("FPS", Engine::Engine::data.fpsSamples, 32);
        ImGui::Text("CPU: %.1fms", (double) Engine::Engine::data.lastCpuThread * 0.001);
        ImGui::Text("CPU thread wait: %.1fms", (double) Engine::Engine::data.cpuWait * 0.001);
        ImGui::PlotLines("CPU thread", Engine::Engine::data.cpuSamples, 256);
        ImGui::Text("GPU: %.1fms", (double) Engine::Engine::data.lastGpuThread * 0.001);
        ImGui::Text("GPU thread wait: %.1fms", (double) Engine::Engine::data.gpuWait * 0.001);
        ImGui::PlotLines("GPU thread", Engine::Engine::data.gpuSamples, 256);
        ImGui::NewLine();
        ImGui::Text("Facing: %.2f %.2f", camera.rotation.x, camera.rotation.y);
        ImGui::Text("Position: %.1f %.1f %.1f", camera.position.x, camera.position.y, camera.position.z);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Game::createGraphicsPipeline() {
    LitPipeline litPipeline(vertShaderModule, fragShaderModule);

    graphicsPipeline = litPipeline.build(pipelineLayout);

    vkDestroyShaderModule(Engine::VulkanContext::device, fragShaderModule, nullptr);
    vkDestroyShaderModule(Engine::VulkanContext::device, vertShaderModule, nullptr);
}

void Game::createPipelineLayout() {
    VkPushConstantRange push_constant = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Uniform_ModelData)};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pPushConstantRanges = &push_constant;
    pipelineLayoutInfo.pushConstantRangeCount = 1;

    if (vkCreatePipelineLayout(Engine::VulkanContext::device, &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void Game::loadAssets() {
    Engine::AssetLoader::loadGameObject(&cubeGameObject, "./data/meshes/cube.obj");
    Engine::AssetLoader::loadGameObject(&planeGameObject, "./data/meshes/plane.obj");

    Engine::AssetLoader::loadShader(&vertShaderModule, "./data/shaders/vert.spv");
    Engine::AssetLoader::loadShader(&fragShaderModule, "./data/shaders/frag.spv");

    Engine::AssetLoader::asyncLoad();
}

void Game::preInit() {

}

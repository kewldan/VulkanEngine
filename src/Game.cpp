#include "Game.h"

#include "io/AssetLoader.h"
#include "graphics/BufferHandler.h"
#include "plog/Log.h"
#include "common/Engine.h"
#include "graphics/VulkanContext.h"
#include "graphics/RenderPipeline.h"

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

    world.init();

    planeGameObject.scale = glm::vec3(30.f, 1.f, 30.f);

    uniformCamera = pipelineLayout.createUniformBuffer<Uniform_CameraData>(VK_SHADER_STAGE_VERTEX_BIT, 0);
    uniformModel = pipelineLayout.createPushBuffer<Uniform_ModelData>(VK_SHADER_STAGE_VERTEX_BIT);

    camera.position = glm::vec3(2.f, 2.f, 2.f);
    camera.rotation = glm::vec3(0.6f, -0.8f, 0.f);

    pipelineLayout.build();
    graphicsPipeline.build(pipelineLayout.getLayout());

    graphicsPipeline.init([this](Engine::GameObject &object) {
        uniformModel.color = object.color;
        uniformModel.model = object.getModel();
        uniformModel.upload(pipelineLayout.getLayout());

        for (int i = 0; i < object.meshCount; i++) {
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(Engine::VulkanContext::commandBuffers[Engine::VulkanContext::currentFrame], 0, 1,
                                   &object.meshes[i].vertexBuffer.buffer, offsets);
            vkCmdBindIndexBuffer(Engine::VulkanContext::commandBuffers[Engine::VulkanContext::currentFrame],
                                 object.meshes[i].indexBuffer.buffer, 0,
                                 VK_INDEX_TYPE_UINT16);

            VkDescriptorSet sets[] = {
                    uniformCamera.getDescriptorSet()
            };
            vkCmdBindDescriptorSets(Engine::VulkanContext::commandBuffers[Engine::VulkanContext::currentFrame],
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipelineLayout.getLayout(), 0, 1,
                                    sets, 0, nullptr);
            vkCmdDrawIndexed(Engine::VulkanContext::commandBuffers[Engine::VulkanContext::currentFrame],
                             object.meshes[i].indexCount, 1, 0, 0, 0);
        }
    });
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
    pipelineLayout.destroy();
    graphicsPipeline.destroy();

    uniformCamera.destroy();
    cubeGameObject.destroy();
    planeGameObject.destroy();
    window->destroy();
}

void Game::createWindow() {
    window = std::make_unique<Engine::Window>("Vulkan game", 1280, 720);
}

void Game::render(VkCommandBuffer commandBuffer) {
    graphicsPipeline.bind();

    Engine::VulkanHelper::updateViewportScissor();

    graphicsPipeline.renderFunction(cubeGameObject);
    graphicsPipeline.renderFunction(planeGameObject);
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

void Game::loadAssets() {
    Engine::AssetLoader::loadAsset(cubeGameObject);
    Engine::AssetLoader::loadAsset(planeGameObject);

    graphicsPipeline.load();

    Engine::AssetLoader::asyncLoad();
}

void Game::preInit() {
    graphicsPipeline = LitPipeline("./data/shaders/vert.spv", "./data/shaders/frag.spv");

    cubeGameObject = Engine::GameObject("./data/meshes/cube.obj");
    cubeGameObject.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    planeGameObject = Engine::GameObject("./data/meshes/plane.obj");
    planeGameObject.color = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
}

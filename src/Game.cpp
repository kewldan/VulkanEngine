#include <vector>
#include <stdexcept>
#include <algorithm>
#include <thread>
#include "Game.h"
#include "imgui.h"
#include "io/AssetLoader.h"
#include "graphics/BufferHandler.h"
#include "plog/Log.h"
#include "common/Engine.h"

void Game::init() {
    Engine::InputManager::setRawInput(window->getHandle(), true);
    Engine::InputManager::setLockCursor(window->getHandle(), true);
    Engine::InputManager::registerCallbacks(window->getHandle());

    Engine::InputManager::keyPressListeners.emplace_back([=, this](int key) {
        if (key == GLFW_KEY_ESCAPE) {
            Engine::InputManager::setLockCursor(window->getHandle(), !Engine::InputManager::lockCursor);
        }
    });

    world.init();

    planeGameObject.scale = glm::vec3(100.f, 1.f, 100.f);

    uniformCamera.init(context.physicalDevice,
                       context.device, context.descriptorPool,
                       VK_SHADER_STAGE_VERTEX_BIT, 0);

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

    camera.update((float) context.swapChainExtent->width, (float) context.swapChainExtent->height);

    uniformCamera.view = camera.getView();
    uniformCamera.proj = camera.getProjection();
    uniformCamera.proj[1][1] *= -1;

    uniformCamera.upload(*context.currentFrame);
}

void Game::cleanup() {
    world.cleanup();
    vkDestroyPipelineLayout(context.device, pipelineLayout, nullptr);
    vkDestroyPipeline(context.device, graphicsPipeline, nullptr);

    for (VkDescriptorSetLayout layout: descriptorSetLayouts) {
        vkDestroyDescriptorSetLayout(context.device, layout, nullptr);
    }

    uniformCamera.cleanup(context.device);
    cubeGameObject.cleanup(context.allocator);
    planeGameObject.cleanup(context.allocator);
    window->cleanup();
}

void Game::createWindow() {
    window = std::make_unique<Engine::Window>("VulkanHelper game", 1280, 720);
}

void Game::render(VkCommandBuffer commandBuffer) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) context.swapChainExtent->width;
    viewport.height = (float) context.swapChainExtent->height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = *context.swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    static Uniform_ModelData data{};

    data.model = cubeGameObject.getModel();
    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Uniform_ModelData),
                       &data);
    for (int i = 0; i < cubeGameObject.meshCount; i++) {
        VkBuffer vertexBuffers[] = {cubeGameObject.meshes[i].vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, cubeGameObject.meshes[i].indexBuffer.buffer, 0,
                             VK_INDEX_TYPE_UINT16);
        VkDescriptorSet sets[] = {
                uniformCamera.descriptorSets[*context.currentFrame]
        };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout, 0, 1,
                                sets, 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, cubeGameObject.meshes[i].indexCount, 1, 0, 0, 0);
    }


    data.model = planeGameObject.getModel();
    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Uniform_ModelData),
                       &data);
    for (int i = 0; i < planeGameObject.meshCount; i++) {
        VkBuffer vertexBuffers[] = {planeGameObject.meshes[i].vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, planeGameObject.meshes[i].indexBuffer.buffer, 0,
                             VK_INDEX_TYPE_UINT16);

        VkDescriptorSet sets[] = {
                uniformCamera.descriptorSets[*context.currentFrame]
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
        ImGui::Text("CPU: %.1fms", (double) Engine::Engine::data.lastCpuThread * 0.001);
        ImGui::Text("GPU: %.1fms", (double) Engine::Engine::data.lastGpuThread * 0.001);
        ImGui::NewLine();
        ImGui::Text("Facing: %.2f %.2f", camera.rotation.x, camera.rotation.y);
        ImGui::Text("Position: %.1f %.1f %.1f", camera.position.x, camera.position.y, camera.position.z);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Game::createGraphicsPipeline() {
    VkShaderModule vertShaderModule = Engine::AssetLoader::loadShader(context.device, "./data/shaders/vert.spv");
    VkShaderModule fragShaderModule = Engine::AssetLoader::loadShader(context.device, "./data/shaders/frag.spv");

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Engine::Vertex::getBindingDescription();
    int count = 0;
    auto attributeDescriptions = Engine::Vertex::getAttributeDescriptions(&count);

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = count;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_8_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.pNext = nullptr;

    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = context.renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(context.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(context.device, fragShaderModule, nullptr);
    vkDestroyShaderModule(context.device, vertShaderModule, nullptr);
}

void Game::createPipelineLayout() {
    VkPushConstantRange push_constant = {VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Uniform_ModelData)};

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = descriptorSetLayouts.size();
    pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
    pipelineLayoutInfo.pPushConstantRanges = &push_constant;
    pipelineLayoutInfo.pushConstantRangeCount = 1;

    if (vkCreatePipelineLayout(context.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void Game::loadAssets() {
    Engine::AssetLoader::loadGameObject(context.allocator, cubeGameObject, "./data/meshes/cube.obj");
    Engine::AssetLoader::loadGameObject(context.allocator, planeGameObject, "./data/meshes/plane.obj");
    Engine::AssetLoader::asyncLoad();
}

#include <stdexcept>
#include "LitPipeline.h"
#include "graphics/VulkanContext.h"
#include "io/AssetLoader.h"

void LitPipeline::build(const Engine::RenderPipelineLayout &layout) {
    Engine::RenderPipeline::build(layout);

    auto vertexInputStage = getVertexInputState();
    auto multisampler = getMultisampling(Engine::VulkanContext::msaaSamples);
    auto colorBlending = getColorBlending();
    auto depthState = getDepthState();
    auto dynamicStates = getDynamicStates();
    auto inputAssembly = getInputAssembly();
    auto viewportState = getViewportStateInfo();
    auto rasterizer = getRasterizer();

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pStages = getShaderStages(&pipelineInfo.stageCount, vertex.getModule(), fragment.getModule());
    pipelineInfo.pVertexInputState = vertexInputStage;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampler;
    pipelineInfo.pColorBlendState = colorBlending;
    pipelineInfo.pDepthStencilState = &depthState;
    pipelineInfo.pDynamicState = &dynamicStates;
    pipelineInfo.layout = layout.getLayout();
    pipelineInfo.renderPass = Engine::VulkanContext::renderPass.getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(Engine::VulkanContext::device, Engine::VulkanContext::pipelineCache, 1, &pipelineInfo,
                                  nullptr,
                                  &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline");
    }

    vertex.destroy();
    fragment.destroy();
}

LitPipeline::LitPipeline(const char *vertexSourceFile,
                         const char *fragmentSourceFile) {
    assert(vertexSourceFile != nullptr);
    assert(fragmentSourceFile != nullptr);

    vertex = Engine::Shader(vertexSourceFile);
    fragment = Engine::Shader(fragmentSourceFile);
}

void LitPipeline::load() {
    Engine::AssetLoader::loadAsset(vertex);
    Engine::AssetLoader::loadAsset(fragment);
}

LitPipeline::LitPipeline() = default;

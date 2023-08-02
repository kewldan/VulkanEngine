#include <stdexcept>
#include "LitPipeline.h"
#include "graphics/VulkanContext.h"

VkPipeline LitPipeline::build(VkPipelineLayout layout) {
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
    pipelineInfo.pStages = getShaderStages(&pipelineInfo.stageCount, vertex, fragment);
    pipelineInfo.pVertexInputState = &vertexInputStage;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampler;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &depthState;
    pipelineInfo.pDynamicState = &dynamicStates;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = Engine::VulkanContext::renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    VkPipeline pipeline = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(Engine::VulkanContext::device, Engine::VulkanContext::pipelineCache, 1, &pipelineInfo,
                                  nullptr,
                                  &pipeline) != VK_SUCCESS) {
        throw new std::runtime_error("failed to create pipeline");
    }

    return pipeline;
}

LitPipeline::LitPipeline(const VkShaderModule vertex, const VkShaderModule fragment) : vertex(vertex),
                                                                                       fragment(fragment) {}

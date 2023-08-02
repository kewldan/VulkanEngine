#include <stdexcept>
#include "graphics/RenderPipeline.h"
#include "graphics/Mesh.h"

namespace Engine {

    VkPipelineInputAssemblyStateCreateInfo RenderPipeline::getInputAssembly() {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;
        return inputAssembly;
    }

    VkPipelineViewportStateCreateInfo RenderPipeline::getViewportStateInfo() {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;
        return viewportState;
    }

    VkPipelineRasterizationStateCreateInfo RenderPipeline::getRasterizer() {
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        return rasterizer;
    }

    const VkPipelineColorBlendStateCreateInfo *RenderPipeline::getColorBlending() {
        auto *attachments = new VkPipelineColorBlendAttachmentState[1];
        attachments[0] = VkPipelineColorBlendAttachmentState();
        attachments[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                                        VK_COLOR_COMPONENT_A_BIT;
        attachments[0].blendEnable = VK_FALSE;

        auto *colorBlending = new VkPipelineColorBlendStateCreateInfo();
        colorBlending->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending->logicOpEnable = VK_FALSE;
        colorBlending->logicOp = VK_LOGIC_OP_COPY;
        colorBlending->attachmentCount = 1;
        colorBlending->pAttachments = attachments;
        colorBlending->blendConstants[0] = 0.0f;
        colorBlending->blendConstants[1] = 0.0f;
        colorBlending->blendConstants[2] = 0.0f;
        colorBlending->blendConstants[3] = 0.0f;
        colorBlending->blendConstants[3] = 0.0f;

        return colorBlending;
    }

    VkPipelineDynamicStateCreateInfo RenderPipeline::getDynamicStates() {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = new VkDynamicState[]{
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
        };

        return dynamicState;
    }

    VkPipelineMultisampleStateCreateInfo RenderPipeline::getMultisampling(VkSampleCountFlagBits msaa) {
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = msaa;

        return multisampling;
    }

    const VkPipelineVertexInputStateCreateInfo *RenderPipeline::getVertexInputState() {
        auto *vertexInputInfo = new VkPipelineVertexInputStateCreateInfo();
        vertexInputInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        auto bindingDescription = Engine::Vertex::getBindingDescription();
        int count = 0;
        auto attributeDescriptions = Engine::Vertex::getAttributeDescriptions(&count);

        vertexInputInfo->vertexBindingDescriptionCount = 1;
        vertexInputInfo->vertexAttributeDescriptionCount = count;
        vertexInputInfo->pVertexBindingDescriptions = bindingDescription;
        vertexInputInfo->pVertexAttributeDescriptions = attributeDescriptions;

        return vertexInputInfo;
    }

    VkPipelineDepthStencilStateCreateInfo RenderPipeline::getDepthState() {
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.pNext = nullptr;

        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f; // Optional
        depthStencil.maxDepthBounds = 1.0f; // Optional
        depthStencil.stencilTestEnable = VK_FALSE;

        return depthStencil;
    }

    VkPipelineShaderStageCreateInfo *
    RenderPipeline::getShaderStages(uint32_t *count, VkShaderModule vertex, VkShaderModule fragment) {
        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.pNext = nullptr;
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertex;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.pNext = nullptr;
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragment;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo *shaderStages = new VkPipelineShaderStageCreateInfo[]{vertShaderStageInfo,
                                                                                              fragShaderStageInfo};

        *count = 2;

        return shaderStages;
    }
}
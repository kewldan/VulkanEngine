#ifndef VULKANENGINE_LITPIPELINE_H
#define VULKANENGINE_LITPIPELINE_H

#include "graphics/RenderPipeline.h"

class LitPipeline : public Engine::RenderPipeline {
private:
    VkShaderModule vertex, fragment;
public:
    LitPipeline(const VkShaderModule vertex, const VkShaderModule fragment);

    VkPipeline build(VkPipelineLayout layout) override;
};


#endif //VULKANENGINE_LITPIPELINE_H

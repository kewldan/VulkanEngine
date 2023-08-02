#ifndef VULKANENGINE_LITPIPELINE_H
#define VULKANENGINE_LITPIPELINE_H

#include "graphics/RenderPipeline.h"
#include "graphics/Shader.h"

class LitPipeline : public Engine::RenderPipeline {
private:
    Engine::Shader vertex, fragment;
public:
    LitPipeline(const char *vertexSourceFile, const char *fragmentSourceFile);

    LitPipeline();

    void build(VkPipelineLayout layout) override;

    void load();
};


#endif //VULKANENGINE_LITPIPELINE_H

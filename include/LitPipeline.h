#ifndef VULKANENGINE_LITPIPELINE_H
#define VULKANENGINE_LITPIPELINE_H

#include "graphics/RenderPipeline.h"

class LitPipeline : public Engine::RenderPipeline {
private:
    VkShaderModule vertex = VK_NULL_HANDLE, fragment = VK_NULL_HANDLE;
    const char *vertexSourceFile{}, *fragmentSourceFile{};
public:
    LitPipeline(const char *vertexSourceFile, const char *fragmentSourceFile);

    LitPipeline();

    void build(VkPipelineLayout layout) override;

    void load();
};


#endif //VULKANENGINE_LITPIPELINE_H

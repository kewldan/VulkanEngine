#ifndef VULKANENGINE_COMMANDBUFFERS_H
#define VULKANENGINE_COMMANDBUFFERS_H

#include "vulkan/vulkan.h"
#include "misc/Destroyable.h"

namespace Engine {
    class CommandBuffers : public Destroyable {
    private:
        VkCommandBuffer commandBuffers[MAX_FRAMES_IN_FLIGHT]{};
    public:
        void init();

        VkCommandBuffer getCommandBuffer();

        void destroy() override;
    };
} // Engine

#endif //VULKANENGINE_COMMANDBUFFERS_H

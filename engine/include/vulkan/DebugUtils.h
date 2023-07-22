#pragma once

#include <vulkan/vulkan.h>

namespace Engine {
    class DebugUtils {
    public:
        static VkDevice m_device;
        static VkInstance instance;

        static void setObjectName(const uint64_t object, const char *name, VkObjectType t) {
            VkDebugUtilsObjectNameInfoEXT s{VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr, t, object,
                                            name};
            PFN_vkSetDebugUtilsObjectNameEXT setDebugUtilsObjectNameExt = VK_NULL_HANDLE;
            setDebugUtilsObjectNameExt = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetInstanceProcAddr(instance,
                                                                                                  "vkSetDebugUtilsObjectNameEXT");
            setDebugUtilsObjectNameExt(m_device, &s);
        }

        static void setObjectName(VkBuffer object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_BUFFER);
        }

        static void setObjectName(VkBufferView object, const char *&name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_BUFFER_VIEW);
        }

        static void setObjectName(VkCommandBuffer object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_COMMAND_BUFFER);
        }

        static void setObjectName(VkCommandPool object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_COMMAND_POOL);
        }

        static void setObjectName(VkDescriptorPool object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_DESCRIPTOR_POOL);
        }

        static void setObjectName(VkDescriptorSet object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_DESCRIPTOR_SET);
        }

        static void setObjectName(VkDescriptorSetLayout object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT);
        }

        static void setObjectName(VkDevice object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_DEVICE);
        }

        static void setObjectName(VkDeviceMemory object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_DEVICE_MEMORY);
        }

        static void setObjectName(VkFramebuffer object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_FRAMEBUFFER);
        }

        static void setObjectName(VkImage object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_IMAGE);
        }

        static void setObjectName(VkImageView object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_IMAGE_VIEW);
        }

        static void setObjectName(VkPipeline object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_PIPELINE);
        }

        static void setObjectName(VkPipelineLayout object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_PIPELINE_LAYOUT);
        }

        static void setObjectName(VkQueryPool object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_QUERY_POOL);
        }

        static void setObjectName(VkQueue object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_QUEUE);
        }

        static void setObjectName(VkRenderPass object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_RENDER_PASS);
        }

        static void setObjectName(VkSampler object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_SAMPLER);
        }

        static void setObjectName(VkSemaphore object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_SEMAPHORE);
        }

        static void setObjectName(VkShaderModule object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_SHADER_MODULE);
        }

        static void setObjectName(VkSwapchainKHR object, const char *name) {
            setObjectName((uint64_t) object, name, VK_OBJECT_TYPE_SWAPCHAIN_KHR);
        }
    };
}

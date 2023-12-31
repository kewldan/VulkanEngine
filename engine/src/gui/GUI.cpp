#include <xutility>
#include "gui/GUI.h"
#include "plog/Log.h"
#include "graphics/DebugUtils.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace Engine {

    void GUI::init(GLFWwindow *window) {
        VkDescriptorPoolSize pool_sizes[] =
                {
                        {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
                        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
                        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
                        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
                        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
                };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        vkCreateDescriptorPool(VulkanContext::device, &pool_info, nullptr, &imguiPool);

#ifndef NDEBUG
        DebugUtils::setObjectName(imguiPool, "ImGui descriptor pool");
#endif

        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForVulkan(window, false);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = VulkanContext::instance;
        init_info.PhysicalDevice = VulkanContext::physicalDevice;
        init_info.Device = VulkanContext::device;
        init_info.Queue = VulkanContext::graphicsQueue;
        init_info.DescriptorPool = imguiPool;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VulkanContext::msaaSamples;

        ImGui_ImplVulkan_Init(&init_info, VulkanContext::renderPass.getRenderPass());

        VkResult err = vkResetCommandPool(VulkanContext::device, VulkanContext::commandPool, 0);
        PLOGF_IF(err != VK_SUCCESS) << err;

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(VulkanContext::commandBuffers.getCommandBuffer(), &begin_info);
        PLOGF_IF(err != VK_SUCCESS) << err;

        ImGui_ImplVulkan_CreateFontsTexture(VulkanContext::commandBuffers.getCommandBuffer());

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;

        VkCommandBuffer commandBuffers[] = {VulkanContext::commandBuffers.getCommandBuffer()};

        end_info.pCommandBuffers = commandBuffers;

        err = vkEndCommandBuffer(VulkanContext::commandBuffers.getCommandBuffer());
        PLOGF_IF(err != VK_SUCCESS) << err;
        err = vkQueueSubmit(VulkanContext::graphicsQueue, 1, &end_info, VK_NULL_HANDLE);
        PLOGF_IF(err != VK_SUCCESS) << err;

        err = vkDeviceWaitIdle(VulkanContext::device);
        PLOGF_IF(err != VK_SUCCESS) << err;
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        ImGui::GetIO().IniFilename = nullptr;
    }

    void GUI::render(VkCommandBuffer buffer) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer);
    }

    VkDescriptorPool GUI::imguiPool = nullptr;

    void GUI::begin() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();
    }

    void GUI::end() {
        ImGui::Render();
    }

    void GUI::destroy() {
        vkDestroyDescriptorPool(VulkanContext::device, imguiPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}


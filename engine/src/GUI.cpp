#include <xutility>
#include "GUI.h"
#include "plog/Log.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace Engine {

    void GUI::init(GLFWwindow *window, VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device,
                   VkQueue graphicsQueue,
                   VkRenderPass renderPass, VkCommandPool commandPool, VkCommandBuffer commandBuffer) {
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


        vkCreateDescriptorPool(device, &pool_info, nullptr, &imguiPool);

        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForVulkan(window, true);

        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice = physicalDevice;
        init_info.Device = device;
        init_info.Queue = graphicsQueue;
        init_info.DescriptorPool = imguiPool;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&init_info, renderPass);

        VkResult err = vkResetCommandPool(device, commandPool, 0);
        PLOGF_IF(err != VK_SUCCESS) << err;

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(commandBuffer, &begin_info);
        PLOGF_IF(err != VK_SUCCESS) << err;

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &commandBuffer;
        err = vkEndCommandBuffer(commandBuffer);
        PLOGF_IF(err != VK_SUCCESS) << err;
        err = vkQueueSubmit(graphicsQueue, 1, &end_info, VK_NULL_HANDLE);
        PLOGF_IF(err != VK_SUCCESS) << err;

        err = vkDeviceWaitIdle(device);
        PLOGF_IF(err != VK_SUCCESS) << err;
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void GUI::render(VkCommandBuffer buffer) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer);
    }

    void GUI::cleanup(VkDevice device) {
        vkDestroyDescriptorPool(device, imguiPool, nullptr);

        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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
}


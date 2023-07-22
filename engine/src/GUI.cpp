#include <xutility>
#include "../GUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace Engine {

    void GUI::init(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device, VkQueue graphicsQueue,
                   VkRenderPass renderPass) {
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

        VkDescriptorPool imguiPool;
        vkCreateDescriptorPool(device, &pool_info, nullptr, &imguiPool);


        // 2: initialize imgui library

        //this initializes the core structures of imgui
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForVulkan(nullptr, true);

        //this initializes imgui for Vulkan
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

        //add to destroy the imgui created structures
//        _mainDeletionQueue.push_function([=]() {
//
//            vkDestroyDescriptorPool(_device, imguiPool, nullptr);
//            ImGui_ImplVulkan_Shutdown();
//        });
    }

    void GUI::uploadFonts(VkCommandBuffer buffer) {
        ImGui_ImplVulkan_CreateFontsTexture(buffer);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void GUI::draw() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
    }

    void GUI::render(VkCommandBuffer buffer) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buffer);
    }
}

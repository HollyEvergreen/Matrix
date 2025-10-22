#pragma once
#include <vulkan/vulkan_raii.hpp>
#include <SDL3/SDL.h>
#include "VulkanContext.h"
#include "Swapchain.h"
class Pipeline {
public:
    Pipeline(const VulkanContext& ctx, const Swapchain& swapchain);

    const vk::raii::RenderPass& getRenderPass() const { return *renderPass; }
    const vk::raii::Pipeline& get() const { return *pipeline; }
    const vk::raii::PipelineLayout& getLayout() const { return *pipelineLayout; }

private:
    void createRenderPass(const VulkanContext& ctx, vk::Format swapchainFormat);
    std::vector<char> readShaderFile(const std::string& filename);
    void createPipeline(const VulkanContext& ctx, vk::Extent2D extent);
    vk::raii::ShaderModule createShaderModule(const VulkanContext& ctx,
        const std::vector<char>& code);

    std::unique_ptr<vk::raii::RenderPass> renderPass;
    std::unique_ptr<vk::raii::PipelineLayout> pipelineLayout;
    std::unique_ptr<vk::raii::Pipeline> pipeline;
};
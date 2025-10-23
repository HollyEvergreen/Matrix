#include "Pipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include "Mesh/Vertex.hpp"

Pipeline::Pipeline(const VulkanContext& ctx, const Swapchain& swapchain) {
    createRenderPass(ctx, swapchain.getFormat());
    createPipeline(ctx, swapchain.getExtent());
}

void Pipeline::createRenderPass(const VulkanContext& ctx, vk::Format swapchainFormat) {
    vk::AttachmentDescription colorAttachment{
        {}, swapchainFormat, vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR
    };

    vk::AttachmentReference colorRef{ 0, vk::ImageLayout::eColorAttachmentOptimal };

    vk::SubpassDescription subpass{
        {}, vk::PipelineBindPoint::eGraphics, {}, colorRef
    };

    vk::SubpassDependency dependency{
        VK_SUBPASS_EXTERNAL, 0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        {}, vk::AccessFlagBits::eColorAttachmentWrite
    };

    vk::RenderPassCreateInfo createInfo{ {}, colorAttachment, subpass, dependency };
    renderPass = std::make_unique<vk::raii::RenderPass>(ctx.getDevice(), createInfo);
}
/// <summary>
/// Loads shader modules from the shaders in the shader list
/// </summary>
/// <param name="filename">path to the shader list to load</param>
/// <returns></returns>
std::vector<vk::ShaderModule> Pipeline::LoadShaders(const std::string& filename) {
    auto f = fopen(filename.c_str(), "r");
    std::vector<std::string> lines = {""};
    char c;
    int line_n = 0;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            lines.emplace_back("");
            line_n++;
        }
        lines[line_n] += c;
    }
    int i = 0;
    for (auto& line : lines) {
        std::cout << i++ << "| " << line << "\n";
    }

}

std::vector<char> Pipeline::readShaderFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        std::cout<< std::filesystem::current_path() << '\n';
        std::cout << "Failed to open shader file: " << filename << "\n";
        throw std::runtime_error("");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    std::cout << "Loaded shader: " << filename << " (" << fileSize << " bytes)" << std::endl;
    return buffer;
}

void Pipeline::createPipeline(const VulkanContext& ctx, vk::Extent2D extent) {
    auto Shaders = LoadShaders(ctx.renderer->GetHomePath().string());
    
    exit(-1);

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = {};


    vk::PipelineVertexInputStateCreateInfo vertexInput;
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInput.vertexBindingDescriptionCount = 1;
    vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInput.pVertexBindingDescriptions = &bindingDescription;
    vertexInput.pVertexAttributeDescriptions = attributeDescriptions.data();

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
        {}, vk::PrimitiveTopology::eTriangleList
    };

    vk::Viewport viewport{ 0, 0, (float)extent.width, (float)extent.height, 0, 1 };
    vk::Rect2D scissor{ {0, 0}, extent };
    vk::PipelineViewportStateCreateInfo viewportState{ {}, viewport, scissor };

    vk::PipelineRasterizationStateCreateInfo rasterizer{
        {}, false, false, vk::PolygonMode::eFill, vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise, false, 0, 0, 0, 1.0f
    };

    vk::PipelineMultisampleStateCreateInfo multisampling{
        {}, vk::SampleCountFlagBits::e1
    };

    vk::PipelineColorBlendAttachmentState colorBlendAttachment{
        false, {}, {}, {}, {}, {}, {},
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    };

    vk::PipelineColorBlendStateCreateInfo colorBlending{
        {}, false, vk::LogicOp::eCopy, colorBlendAttachment
    };

    vk::PipelineLayoutCreateInfo layoutInfo{};
    pipelineLayout = std::make_unique<vk::raii::PipelineLayout>(ctx.getDevice(), layoutInfo);

    vk::GraphicsPipelineCreateInfo pipelineInfo{
        {}, shaderStages, &vertexInput, &inputAssembly, nullptr, &viewportState,
        &rasterizer, &multisampling, nullptr, &colorBlending, nullptr,
        **pipelineLayout, **renderPass, 0
    };

    pipeline = std::make_unique<vk::raii::Pipeline>(ctx.getDevice(), nullptr, pipelineInfo);
}

vk::raii::ShaderModule Pipeline::createShaderModule(const VulkanContext& ctx,
    const std::vector<char>& code) {
    vk::ShaderModuleCreateInfo createInfo{
        {}, code.size(), reinterpret_cast<const uint32_t*>(code.data())
    };
    return { ctx.getDevice(), createInfo };
}

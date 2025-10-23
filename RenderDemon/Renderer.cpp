#include "Renderer.hpp"
#include <iostream>
#include <array>
#include "Mesh/Vertex.hpp"

Renderer::Renderer(AppCreationContext* a_ctx)
{
	this->AppName = a_ctx->AppName;
	this->HomePath = a_ctx->HomePath;
	this->m_window = new Window(a_ctx->w_ctx);
	this->context = VulkanContext(m_window->GetHandle());
	createSurface();
	swapchain = std::make_unique<Swapchain>(context, *surface, m_window->GetHandle());
	pipeline = new Pipeline(context, *swapchain);
	createCommandPool();
	createFramebuffers();
	createFrameResources();
	
}

Renderer::~Renderer()
{/*
	for (auto& buf : buffers) {
		auto& device = *this->context.getDevice();
		vkDestroyBuffer(device, buf, nullptr);
	}
	for (auto& mem : deviceMemory) {
		auto& device = *this->context.getDevice();
		vkFreeMemory(device, mem, nullptr);
	}*/
	swapchain.release();
}

void Renderer::createSurface() {
	VkSurfaceKHR surf;
	if (!SDL_Vulkan_CreateSurface(m_window->GetHandle(), *context.getInstance(), nullptr, &surf)) {
		throw std::runtime_error("Failed to create surface");
	}
	surface = std::make_unique<vk::raii::SurfaceKHR>(context.getInstance(), surf);
}

void Renderer::createCommandPool() {
	vk::CommandPoolCreateInfo poolInfo{
		vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		context.getGraphicsQueueFamily()
	};
	commandPool = std::make_unique<vk::raii::CommandPool>(context.getDevice(), poolInfo);
}

void Renderer::recordCommandBuffer(vk::CommandBuffer cmd, uint32_t imageIndex) {
	cmd.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });
	vk::ClearValue clearValue;
	clearValue.color = vk::ClearColorValue{
		m_window->
			GetContextHandle()->
				clearColor
	};

	vk::RenderPassBeginInfo renderPassInfo{};
	renderPassInfo.renderPass = *pipeline->getRenderPass();
	renderPassInfo.framebuffer = *framebuffers[imageIndex];
	renderPassInfo.renderArea = vk::Rect2D{ {0, 0}, swapchain->getExtent() };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearValue;


	cmd.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline->get());
	//cmd.bindVertexBuffers(0, buffers[CurrSceneID], {0});
	//cmd.draw()
	cmd.endRenderPass();
	cmd.end();
}

void Renderer::createFramebuffers() {
	framebuffers.clear();
	framebuffers.reserve(swapchain->getImageViews().size());

	for (const auto& imageView : swapchain->getImageViews()) {
		vk::FramebufferCreateInfo createInfo{
			{}, *pipeline->getRenderPass(), *imageView,
			swapchain->getExtent().width, swapchain->getExtent().height, 1
		};
		framebuffers.emplace_back(context.getDevice(), createInfo);
	}
}

void Renderer::createFrameResources() {
	frameResources.clear();
	frameResources.reserve(MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		frameResources.emplace_back(context, *commandPool);
	}
}

void Renderer::createVertexBuffer(const std::vector<Vertex>& vertices)
{}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
	vk::PhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(*this->context.getPhysicalDevice(), &*memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && memProperties.memoryTypes[i].propertyFlags == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

}

void Renderer::drawFrame() {
	auto& frame = frameResources[currentFrame];

	auto result = context.getDevice().waitForFences(*frame.inFlight, VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	auto acquireResult = swapchain->get().acquireNextImage(UINT64_MAX, *frame.imageAvailable);
	imageIndex = acquireResult.second;

	if (acquireResult.first == vk::Result::eErrorOutOfDateKHR) {
		handleResize();
		return;
	}

	context.getDevice().resetFences(*frame.inFlight);
	frame.commandBuffer.reset();
	recordCommandBuffer(*frame.commandBuffer, imageIndex);

	vk::Semaphore waitSems[] = { *frame.imageAvailable };
	vk::Semaphore signalSems[] = { *frame.renderFinished };
	vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer cmdBufs[] = { *frame.commandBuffer };

	vk::SubmitInfo submitInfo{ waitSems, waitStages, cmdBufs, signalSems };
	context.getGraphicsQueue().submit(submitInfo, *frame.inFlight);

	vk::SwapchainKHR swapchains[] = { *swapchain->get() };
	vk::PresentInfoKHR presentInfo{ signalSems, swapchains, imageIndex };

	auto presentResult = context.getPresentQueue().presentKHR(presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::waitIdle() {
	context.getDevice().waitIdle();
}

void Renderer::handleResize() {
	waitIdle();
	swapchain = std::make_unique<Swapchain>(context, *surface, m_window->GetHandle(), swapchain.get());
	pipeline = new Pipeline(context, *swapchain);
	createFramebuffers();
}
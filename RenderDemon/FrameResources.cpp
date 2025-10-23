#include "FrameResources.h"

FrameResources::FrameResources(const VulkanContext& ctx, const vk::raii::CommandPool& pool)
    : commandBuffer(std::move(vk::raii::CommandBuffers(ctx.getDevice(),
        { *pool, vk::CommandBufferLevel::ePrimary, 1 }).front())),
    imageAvailable(ctx.getDevice(), vk::SemaphoreCreateInfo{}),
    renderFinished(ctx.getDevice(), vk::SemaphoreCreateInfo{}),
    inFlight(ctx.getDevice(), vk::FenceCreateInfo{ vk::FenceCreateFlagBits::eSignaled }) {
}

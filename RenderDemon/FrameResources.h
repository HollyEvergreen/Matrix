#pragma once
#include <vulkan/vulkan_raii.hpp>
#include <SDL3/SDL.h>
#include "VulkanContext.h"

struct FrameResources {
    vk::raii::CommandBuffer commandBuffer;
    vk::raii::Semaphore imageAvailable;
    vk::raii::Semaphore renderFinished;
    vk::raii::Fence inFlight;

    FrameResources(const VulkanContext& ctx, const vk::raii::CommandPool& pool);
};
#pragma once
#include <vulkan/vulkan_raii.hpp>
#include <SDL3/SDL.h>
#include "VulkanContext.h"
class Swapchain {
public:
    Swapchain(const VulkanContext& ctx, const vk::raii::SurfaceKHR& surface,
        SDL_Window* window, const Swapchain* oldSwapchain = nullptr);

    const vk::raii::SwapchainKHR& get() const { return *swapchain; }
    const std::vector<vk::raii::ImageView>& getImageViews() const { return imageViews; }
    vk::Format getFormat() const { return surfaceFormat.format; }
    vk::Extent2D getExtent() const { return extent; }
    size_t getImageCount() const { return images.size(); }

private:
    void createSwapchain(const VulkanContext& ctx, const vk::raii::SurfaceKHR& surface,
        SDL_Window* window, const Swapchain* oldSwapchain);
    void createImageViews(const VulkanContext& ctx);

    std::unique_ptr<vk::raii::SwapchainKHR> swapchain;
    std::vector<vk::Image> images;
    std::vector<vk::raii::ImageView> imageViews;
    vk::SurfaceFormatKHR surfaceFormat;
    vk::PresentModeKHR presentMode;
    vk::Extent2D extent;
};
#include "Swapchain.h"
#include <algorithm>

Swapchain::Swapchain(const VulkanContext& ctx, const vk::raii::SurfaceKHR& surface,
    SDL_Window* window, const Swapchain* oldSwapchain) {
    createSwapchain(ctx, surface, window, oldSwapchain);
    createImageViews(ctx);
}

void Swapchain::createSwapchain(const VulkanContext& ctx, const vk::raii::SurfaceKHR& surface,
    SDL_Window* window, const Swapchain* oldSwapchain) {
    auto capabilities = ctx.getPhysicalDevice().getSurfaceCapabilitiesKHR(*surface);
    auto formats = ctx.getPhysicalDevice().getSurfaceFormatsKHR(*surface);
    auto presentModes = ctx.getPhysicalDevice().getSurfacePresentModesKHR(*surface);

    // Choose format
    surfaceFormat = formats[0];
    for (const auto& fmt : formats) {
        if (fmt.format == vk::Format::eB8G8R8A8Srgb &&
            fmt.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            surfaceFormat = fmt;
            break;
        }
    }

    // Choose present mode
    presentMode = vk::PresentModeKHR::eFifo;
    for (const auto& mode : presentModes) {
        if (mode == vk::PresentModeKHR::eMailbox) {
            presentMode = mode;
            break;
        }
    }

    // Choose extent
    if (capabilities.currentExtent.width != UINT32_MAX) {
        extent = capabilities.currentExtent;
    }
    else {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        extent.width = std::clamp(static_cast<uint32_t>(w),
            capabilities.minImageExtent.width,
            capabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<uint32_t>(h),
            capabilities.minImageExtent.height,
            capabilities.maxImageExtent.height);
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0) {
        imageCount = std::min(imageCount, capabilities.maxImageCount);
    }

    vk::SwapchainCreateInfoKHR createInfo{
        {}, *surface, imageCount, surfaceFormat.format, surfaceFormat.colorSpace,
        extent, 1, vk::ImageUsageFlagBits::eColorAttachment
    };

    uint32_t queueFamilies[] = { ctx.getGraphicsQueueFamily(), ctx.getPresentQueueFamily() };
    if (queueFamilies[0] != queueFamilies[1]) {
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilies;
    }
    else {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = oldSwapchain ? *oldSwapchain->get() : nullptr;

    swapchain = std::make_unique<vk::raii::SwapchainKHR>(ctx.getDevice(), createInfo);
    images = swapchain->getImages();
}

void Swapchain::createImageViews(const VulkanContext& ctx) {
    imageViews.reserve(images.size());

    for (const auto& image : images) {
        vk::ImageViewCreateInfo createInfo{
            {}, image, vk::ImageViewType::e2D, surfaceFormat.format,
            {}, {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
        };
        imageViews.emplace_back(ctx.getDevice(), createInfo);
    }
}
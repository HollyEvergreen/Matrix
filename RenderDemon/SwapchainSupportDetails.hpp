#include <vulkan/vulkan.hpp>

struct SwapchainSupportDetails {
	vk::SurfaceCapabilitiesKHR SurfaceCapabilities;
	std::vector<vk::SurfaceFormatKHR> SurfaceFormats;
	std::vector<vk::PresentModeKHR> PresentationModes;
	SwapchainSupportDetails() {}
	SwapchainSupportDetails(vk::PhysicalDevice& _device, vk::SurfaceKHR& _surface) {
		// vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
		SurfaceCapabilities = _device.getSurfaceCapabilitiesKHR(_surface);
		SurfaceFormats = _device.getSurfaceFormatsKHR(_surface);
		PresentationModes = _device.getSurfacePresentModesKHR(_surface);
	}

	constexpr bool isSwapchainAdequate() const {
		return !SurfaceFormats.empty() && !PresentationModes.empty();
	}

	constexpr vk::SurfaceFormatKHR chooseSwapSurfaceFormat() {
		for (const auto& Format : SurfaceFormats) { // VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
			if (Format.format == vk::Format::eB8G8R8A8Srgb && Format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
				return Format;
			}
		}
		return SurfaceFormats[0];
	}
	constexpr vk::PresentModeKHR choosePresentationMode() {
		for (const auto& Mode : PresentationModes) {
			if (Mode == vk::PresentModeKHR::eMailbox) {
				return Mode;
			}
		}
		return vk::PresentModeKHR::eFifo;
	}
	vk::Extent2D chooseSwapSurfaceExtents() const {
		if (SurfaceCapabilities.currentExtent.width != INT32_MAX) {
			return SurfaceCapabilities.currentExtent;
		}
		else {
			int width, height;
			auto win = RenderContext::Get<GLFWwindow*>("Glfw-Window");
			glfwGetFramebufferSize(win, &width, &height);

			vk::Extent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, SurfaceCapabilities.minImageExtent.width, SurfaceCapabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, SurfaceCapabilities.minImageExtent.height, SurfaceCapabilities.maxImageExtent.height);

			return actualExtent;
		}
	}
};

#include <vulkan/vulkan.hpp>
#include "Devices.hpp"
#include "Surface.hpp"

class Swapchain {
	vk::SwapchainKHR m_SwapchainKHR;
	std::vector<vk::Image> m_SwapchainImages;

	SwapchainSupportDetails support;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::PresentModeKHR presentMode;
	vk::Extent2D extent;
	uint32_t imageCount;
public:
	Swapchain(Device& device, vk::SurfaceKHR& surface) {
		support = SwapchainSupportDetails(device._physical_device->GetDevice(), surface);
		surfaceFormat = support.chooseSwapSurfaceFormat();
		presentMode = support.choosePresentationMode();
		extent = support.chooseSwapSurfaceExtents();
		imageCount = support.SurfaceCapabilities.minImageCount + 1;
		if (support.SurfaceCapabilities.maxImageCount != 0 and support.SurfaceCapabilities.maxImageCount < imageCount) imageCount = support.SurfaceCapabilities.maxImageCount;

		vk::SwapchainCreateInfoKHR info = vk::SwapchainCreateInfoKHR();
		info.setSurface(surface);
		info.setMinImageCount(imageCount);
		info.setImageFormat(surfaceFormat.format);
		info.setImageColorSpace(surfaceFormat.colorSpace);
		info.setImageExtent(extent);
		info.setImageArrayLayers(1);
		info.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
		uint32_t queueFamilyIndices = device.getIndices().GetGraphicsFamily().value();
		info.setImageSharingMode(vk::SharingMode::eExclusive);
		info.setQueueFamilyIndexCount(0);
		info.setQueueFamilyIndices(nullptr);
		info.setPreTransform(support.SurfaceCapabilities.currentTransform);
		info.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
		info.setPresentMode(presentMode);
		info.setClipped(vk::True);
		info.setOldSwapchain(nullptr);

		try {
			m_SwapchainKHR = device.GetDevice()->createSwapchainKHR(info);
			std::cout << "Swapchain created successfully at <"<< ColourCodes[ConsoleColour::RED]<<"0x" << &m_SwapchainKHR << ColourCodes[ConsoleColour::GREEN] << ">\n";
			m_SwapchainImages = device.GetDevice()->getSwapchainImagesKHR(m_SwapchainKHR);
		}
		catch (vk::SystemError err) {
			std::cout << err.code() << " " << err.what() << std::endl;
		}
	}
};
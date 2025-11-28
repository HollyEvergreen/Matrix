#include <vulkan/vulkan.hpp>
#include "Devices.hpp"
#include "Surface.hpp"

class Swapchain {
	vk::SwapchainKHR m_SwapchainKHR;
	std::vector<vk::Image> m_SwapchainImages;
	std::vector<vk::UniqueImageView> m_SwapchainImageViews;

	vk::ImageView& getView(int i) {
		return *m_SwapchainImageViews[i];
	}

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
			
		}
		catch (vk::SystemError err) {
			std::cout << err.code() << " " << err.what() << std::endl;
			std::exit(-1);
		}
		m_SwapchainImages = device.GetDevice()->getSwapchainImagesKHR(m_SwapchainKHR);
		m_SwapchainImageViews.resize(m_SwapchainImages.size());
		int i = 0;
		for (auto& image : m_SwapchainImages) {
			auto info = vk::ImageViewCreateInfo();
			info.setImage(image)
				.setViewType(vk::ImageViewType::e2D)
				.setFormat(surfaceFormat.format)
				.setComponents(
					vk::ComponentMapping{
						vk::ComponentSwizzle::eIdentity,
						vk::ComponentSwizzle::eIdentity,
						vk::ComponentSwizzle::eIdentity,
						vk::ComponentSwizzle::eIdentity
					})
				.setSubresourceRange(vk::ImageSubresourceRange{
						vk::ImageAspectFlagBits::eColor,
						0,
						1,
						0,
						1
					});
			try {
				m_SwapchainImageViews[i++] = device.GetDevice()->createImageViewUnique(info);
				std::cout << "Image view " << i-1 << " created successfully at <" << style_static(&m_SwapchainImageViews[i - 1], RED) << ">\n";
			}
			catch (vk::SystemError err){
				std::cout << "Image view <" << i-1 << "> Failed to be created\n" << err.code() << "\n" << err.what() << "\n";
				std::exit(-1);
			}

		}
	}
};
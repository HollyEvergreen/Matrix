#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Matrix.hpp>
#include <RenderContext.hpp>

class QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
public:
	const std::optional<uint32_t>& GetGraphicsFamily() {
		return graphicsFamily;
	}
	uint32_t unwrap() {
		if (!graphicsFamily.has_value())
			throw std::runtime_error("graphicsFamily Is Null");
		else return *graphicsFamily;
	}
	bool hasValue(int i = -1) {
		return graphicsFamily.has_value();
	}
	void SetGraphicsFamily(uint32_t n) { graphicsFamily = n; }
};

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice* device, vk::SurfaceKHR surface) {
	QueueFamilyIndices indices;
	auto queueFamilyProps = device->getQueueFamilyProperties();
	int i = 0;
	for (const auto& queueFamily : queueFamilyProps) {
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.SetGraphicsFamily(i);
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(
				to_vk_native(vk::PhysicalDevice, *device),
				i, 
				to_vk_native(vk::SurfaceKHR, surface),
				&presentSupport
			);
			if (presentSupport) {
				return indices;
			}
		}
		i++;
	}
	return indices;
}
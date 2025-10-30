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
			throw std::runtime_error("graphicsFamilyIsNull");
		else return *graphicsFamily;
	}
	bool hasValue() {
		return graphicsFamily.has_value();
	}
	void Set(uint32_t n) { graphicsFamily = n; }
};

QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice* device) {
	QueueFamilyIndices indices;
	auto queueFamilyProps = device->getQueueFamilyProperties();
	int i = 0;
	for (const auto& queueFamily : queueFamilyProps) {
		if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.Set(i);
			return indices;
		}
		i++;
	}
	return indices;
}

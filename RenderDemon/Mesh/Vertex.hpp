#pragma once
#include <vulkan/vulkan_raii.hpp>
#include <array>

struct Vertex{
	std::array<float, 3> pos;
	std::array<float, 3> col;

	static constexpr vk::VertexInputBindingDescription getBindingDescription() {
		vk::VertexInputBindingDescription bindingDescription =
			vk::VertexInputBindingDescription(
				0,
				sizeof(Vertex),
				vk::VertexInputRate::eVertex
			);

		return bindingDescription;
	}
	static constexpr std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
		std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {
			vk::VertexInputAttributeDescription(
				0,
				0,
				vk::Format::eR32G32B32Sfloat,
				offsetof(Vertex, pos)
			),
			vk::VertexInputAttributeDescription(
				0,
				1,
				vk::Format::eR32G32B32Sfloat,
				offsetof(Vertex, col)
			),
		};

		return attributeDescriptions;
	}
};
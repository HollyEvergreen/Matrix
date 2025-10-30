#pragma once
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <Matrix.hpp>
#include <vulkan/vulkan.hpp>

class VulkanContext
{
public:
	VulkanContext(
		ImGui_ImplVulkan_InitInfo& initInfo
	) {
		/*RenderContext::Insert(
			"Vulkan-Context", 
			std::make_shared<VulkanContext>(this)
		);
		*/
	}
};

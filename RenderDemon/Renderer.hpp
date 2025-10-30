#pragma once
#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>
#include <Matrix.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanContext.hpp"
#include "Devices.hpp"
#include "Surface.hpp"
#include <typeinfo>


class Renderer
{
public:
	GLFWwindow* _win;
	Device* vkDeviceHnd;
	Surface* RenderSurface;
	const bool enableValidation = true;
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation",
	};

	Renderer() {
		std::bitset<8> windowFlags = 0;//AppContext::Get<std::bitset<8>>("Window-Flags");
		glfwInit();
		glfwSetErrorCallback(GlfwErrorCallback);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(vsync, windowFlags.test(0));
		glfwWindowHint(GLFW_RESIZABLE, windowFlags.test(1));
		_win = glfwCreateWindow(
			AppContext::Get<int>("Window-Width"),
			AppContext::Get<int>("Window-Height"),
			AppContext::Get<std::string>("App-Name").c_str(),
			nullptr,
			nullptr
		);
		RenderContext::Insert("Glfw-Window", _win);

		auto instExtProps = vk::enumerateInstanceExtensionProperties();
		std::cout << ColourCodes[RED] << instExtProps.size() << ColourCodes[GREEN] << " Extensions Supported\n";
		int i = 0;
		vkInstanceHnd = VulkanInstance();

		vkDeviceHnd = new Device(vkInstanceHnd);

		RenderSurface = new Surface(&vkInstanceHnd, _win);

		
		std::string _class_name = typeid(*this).name();
		std::cout << std::format("Exit {} Constructor\n", style(_class_name, RED));
	}

	Renderer& Mainloop() const {
		while (!glfwWindowShouldClose(_win)){
			glfwPollEvents();
		}
	}

	~Renderer() {
		RenderContext::Remove("Glfw-Window");
		glfwDestroyWindow(_win);
		glfwTerminate();
	}

private:
	VulkanInstance vkInstanceHnd;
	static void GlfwErrorCallback(int err, const char* desc) {
		std::cout << std::format("{0}: {1}", err, desc);
	}
};

#pragma once
#pragma warning(push, 0)
#include <vulkan/vulkan.hpp>
#pragma warning(pop)
#include <Matrix.hpp>

class Surface{
	vk::SurfaceKHR _surface;
public:
    Surface(VulkanInstance* _inst, GLFWwindow* _win) {
        VkSurfaceKHR tempSurface;
        VkResult result = glfwCreateWindowSurface(
            to_vk_native(vk::Instance,_inst->Get()),
            _win,
            nullptr,
            &tempSurface
        );

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }

        _surface = vk::SurfaceKHR(tempSurface);
        std::cout << ColourCodes[GREEN] << "Surface created at <"<< ColourCodes[RED] <<"0x" << std::uppercase << std::hex << &_surface << ColourCodes[GREEN] << ">\n";
    }
};
#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Matrix.hpp>
#include <RenderContext.hpp>

class VulkanInstance
{
public:
	VulkanInstance() {
		std::vector<const char*> Layers = std::vector<const char*>{ "VK_LAYER_KHRONOS_validation"};
		std::vector<const char*> EnabledExtensions;
		
		auto extProps = vk::enumerateInstanceExtensionProperties();
		std::vector<const char*> extNames = {};
		for (auto& extName : extProps)
		{
			extNames.emplace_back(extName.extensionName);
		}
		
		extNames.emplace_back("VK_EXT_debug_utils");

		auto appinfo = vk::ApplicationInfo{
			AppContext::Get<std::string>("App-Name").c_str(), 0,
			"Matrix", 0,
			VK_API_VERSION_1_4
		};
		vk::InstanceCreateInfo info = vk::InstanceCreateInfo(
			(vk::InstanceCreateFlags)0,
			&appinfo,
			Layers,
			extNames
		);

		_inst = vk::createInstance(info);
	}

	const vk::Instance& Get() {
		return _inst;
	}

	~VulkanInstance() {
		
	}
private:
	vk::Instance _inst;
};

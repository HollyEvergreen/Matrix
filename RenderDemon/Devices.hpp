#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Matrix.hpp>
#include <RenderContext.hpp>

#include "QueueFamilyIndices.hpp"
#include "Surface.hpp"
#include "SwapchainSupportDetails.hpp"
#include <Predicates.hpp>

class PhysicalDevice {
public:
	PhysicalDevice(VulkanInstance instance, vk::SurfaceKHR& surface) {
		auto devices = instance.Get().enumeratePhysicalDevices();
		std::array<int, 2> bestGraphicsDevice = {-1, -1};
		int i = 0;
		for (auto& device : devices) {
			auto suitability = CalculateSuitability(device, surface);
			if (suitability > bestGraphicsDevice[1]) 
				bestGraphicsDevice = { i, suitability };
			i++;
		}
		_device = std::move(devices[bestGraphicsDevice[0]]);
		std::cout << "Renderer chose <" << ColourCodes[RED] << _device.getProperties().deviceName << ColourCodes[GREEN] << ">" << '\n';
	}
	vk::PhysicalDevice& GetDevice() {
		return _device;
	}
	const vk::PhysicalDevice& GetCDevice() {
		return _device;
	}
	vk::PhysicalDevice* GetHandle() {
		return &_device;
	}
	const vk::PhysicalDevice* GetCHandle() {
		return &_device;
	}

	operator vk::PhysicalDevice() const {
		return _device;
	} operator vk::PhysicalDevice::NativeType() const {
		return static_cast<vk::PhysicalDevice::NativeType>(_device);
	}
	
private:
	vk::PhysicalDevice _device;

	int CalculateSuitability(vk::PhysicalDevice& device, vk::SurfaceKHR& surface) {
		int suitability = 0;

		auto Props = device.getProperties();

		vk::PhysicalDeviceFeatures Features = std::move(device.getFeatures());
		auto _Features = (VkBool32*)(&Features);
		auto QFProps = device.getQueueFamilyProperties();

		switch (Props.deviceType)
		{
		case vk::PhysicalDeviceType::eDiscreteGpu:
			suitability += 2;
			break;
		case vk::PhysicalDeviceType::eIntegratedGpu:
			suitability += 1;
			break;
		default:
			suitability -= 2;
			break;
		}

		std::vector<int> Required = {4, 5};

		for (size_t i = 0; i < 55; i++)
		{
			suitability += _Features[i];
		}
		for (auto n : Required) if (!_Features[n]) suitability = -1;
		SwapchainSupportDetails details = SwapchainSupportDetails(device, surface);
		return suitability * (int)findQueueFamilies(&device, surface).hasValue() * (int)this->checkDeviceExtensionSupport(device) * (int)details.isSwapchainAdequate();
	}
	bool checkDeviceExtensionSupport(vk::PhysicalDevice device) {
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		auto availableExtensions = device.enumerateDeviceExtensionProperties();
		
		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	
};

class Device {
public:
	Device(VulkanInstance _inst, vk::SurfaceKHR& surface) {
		_physical_device = new PhysicalDevice(_inst, surface);
		std::vector<const char*> reqExtensions = {
			"VK_KHR_swapchain"
		};
		indices = findQueueFamilies(_physical_device->GetHandle(), surface);

		vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo(); {
			queueCreateInfo.setQueueFamilyIndex(indices.unwrap());
			queueCreateInfo.setQueueCount(1);
			float queuePriorities[] = {1.0f};
			queueCreateInfo.setPQueuePriorities(queuePriorities);
		}

		auto deviceExtensions = _physical_device->GetDevice().enumerateDeviceExtensionProperties();
		std::vector<const char*> deviceExtensionNames = std::vector<const char*>();
		std::for_each(deviceExtensions.begin(), deviceExtensions.end(), [&reqExtensions, &deviceExtensionNames](vk::ExtensionProperties& ext) {
			auto extName = (const char*)ext.extensionName.data();
			if (Matrix::Predicates::In<const char*>(reqExtensions, extName)) {
				deviceExtensionNames.push_back(ext.extensionName);
			}
		});
		vk::DeviceCreateInfo CreateInfo = vk::DeviceCreateInfo(); {
			CreateInfo.setQueueCreateInfos({ queueCreateInfo });
			vk::PhysicalDeviceFeatures features[] = { _physical_device->GetHandle()->getFeatures() };
			CreateInfo.setPEnabledFeatures(features);
			CreateInfo.setEnabledExtensionCount((uint32_t)deviceExtensionNames.size());
			CreateInfo.setPEnabledExtensionNames(deviceExtensionNames);
			CreateInfo.setEnabledLayerCount(0);
		}
		
		_logical_device = new vk::Device(_physical_device->GetDevice().createDevice(CreateInfo));
	}
	vk::Device* GetDevice() {
		return _logical_device;
	}
	VkQueue GetQueue() {
		return _logical_device->getQueue(indices.unwrap(), 0);
	}

	QueueFamilyIndices getIndices() {
		return indices;
	}

	PhysicalDevice* _physical_device;

	operator PhysicalDevice*() {
		return _physical_device;
	}
	operator vk::Device() {
		return *_logical_device;
	} 
	operator vk::Device::NativeType() {
		return static_cast<vk::Device::NativeType>(*_logical_device);
	}
	operator vk::PhysicalDevice() {
		return *_physical_device;
	} 
	operator vk::PhysicalDevice::NativeType() {
		return static_cast<vk::PhysicalDevice::NativeType>(*_physical_device);
	}
private:
	vk::Device* _logical_device;
	QueueFamilyIndices indices;
};
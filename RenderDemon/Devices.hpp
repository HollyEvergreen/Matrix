#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <Matrix.hpp>
#include <RenderContext.hpp>

#include "QueueFamilyIndices.hpp"

class PhysicalDevice {
public:
	PhysicalDevice(VulkanInstance instance) {
		auto devices = instance.Get().enumeratePhysicalDevices();
		std::array<int, 2> bestGraphicsDevice = {-1, -1};
		int i = 0;
		for (auto& device : devices) {
			auto suitability = CalculateSuitability(device);
			if (suitability > bestGraphicsDevice[1]) 
				bestGraphicsDevice = { i, suitability };
			i++;
		}
		_device = std::move(devices[bestGraphicsDevice[0]]);
		std::cout << "Renderer chose <" << ColourCodes[RED] << _device.getProperties().deviceName << ColourCodes[GREEN] << ">" << '\n';
	}

	const vk::PhysicalDevice& GetDevice() {
		return _device;
	}
	vk::PhysicalDevice* GetHandle() {
		return &_device;
	}
	const vk::PhysicalDevice* GetCHandle() {
		return &_device;
	}
private:
	vk::PhysicalDevice _device;

	int CalculateSuitability(vk::PhysicalDevice device) {
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
		return suitability * (int)findQueueFamilies(&device).hasValue();
	}
};

class Device {
public:
	Device(VulkanInstance _inst) {
		_physical_device = new PhysicalDevice(_inst);

		QueueFamilyIndices indices = findQueueFamilies(_physical_device->GetHandle());

		vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo(); {
			queueCreateInfo.setQueueFamilyIndex(indices.unwrap());
			queueCreateInfo.setQueueCount(1);
			float queuePriorities[] = {1.0f};
			queueCreateInfo.setPQueuePriorities(queuePriorities);
		}

		vk::DeviceCreateInfo CreateInfo = vk::DeviceCreateInfo(); {
			CreateInfo.setQueueCreateInfos({ queueCreateInfo });
			vk::PhysicalDeviceFeatures features[] = { _physical_device->GetHandle()->getFeatures() };
			CreateInfo.setPEnabledFeatures(features);
			CreateInfo.setEnabledExtensionCount(0);
			CreateInfo.setEnabledLayerCount(0);
		}
		
		_logical_device = std::make_unique<vk::Device>(_physical_device->GetDevice().createDevice(CreateInfo));
	}

	VkQueue& GetQueue(int QueueFamilyIndex) {
		assert(QueueFamilyIndex > 0);
		if (_queues.size() > QueueFamilyIndex) {
			_queues.push_back(_logical_device->getQueue(QueueFamilyIndex, 0));
		}
		return _queues[QueueFamilyIndex];
	}

private:
	PhysicalDevice* _physical_device;
	std::unique_ptr<vk::Device> _logical_device;
	std::vector<VkQueue> _queues;
};



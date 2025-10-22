#include "VulkanContext.h"
#include <set>
#include <SDL3/SDL_vulkan.h>
#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::string severity;
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        severity = "[ERROR]";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        severity = "[WARNING]";
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        severity = "[INFO]";
    }
    else {
        severity = "[VERBOSE]";
    }

    std::cerr << severity << " Validation Layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

VulkanContext::VulkanContext()
{
    return;
}

VulkanContext::VulkanContext(SDL_Window* window) {
    createInstance(window);

    std::cout << "Creating Vulkan surface from SDL3 window..." << std::endl;

    // Create temporary surface for device selection
    VkSurfaceKHR tempSurface = nullptr;
    if (!SDL_Vulkan_CreateSurface(window, static_cast<VkInstance>(**instance), nullptr, &tempSurface)) {
        std::string error = SDL_GetError();
        throw std::runtime_error("Failed to create Vulkan surface from SDL window: " + error);
    }

    if (!tempSurface) {
        throw std::runtime_error("SDL_Vulkan_CreateSurface returned null surface");
    }

    std::cout << "Surface created successfully" << std::endl;

    vk::raii::SurfaceKHR surface(*instance, tempSurface);

    pickPhysicalDevice(surface);
    createLogicalDevice();
}

void VulkanContext::createInstance(SDL_Window* window) {
    uint32_t extCount = 0;
    const char* const* extNames = SDL_Vulkan_GetInstanceExtensions(&extCount);
    
    if (!extNames) {
        throw std::runtime_error("SDL_Vulkan_GetInstanceExtensions returned nullptr");
    }
    
    std::vector<const char*> extensions;
    for (uint32_t i = 0; i < extCount; i++) {
        extensions.push_back(extNames[i]);
        std::cout << "Required SDL extension: " << extNames[i] << std::endl;
    }
    
    // Add debug utils extension
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    
    // Query available layers and filter out unwanted ones
    auto availableLayers = context.enumerateInstanceLayerProperties();
    std::vector<const char*> enabledLayers;
    
    std::cout << "Available Vulkan Layers:" << std::endl;
    for (const auto& layer : availableLayers) {
        std::cout << "  - " << layer.layerName << std::endl;
        
        // Only enable validation layer, skip overlay layers
        if (strcmp(layer.layerName, "VK_LAYER_KHRONOS_validation") == 0) {
            enabledLayers.push_back(layer.layerName);
        }
    }
    
    vk::ApplicationInfo appInfo{
        "Vulkan Renderer", VK_MAKE_VERSION(1, 0, 0),
        "No Engine", VK_MAKE_VERSION(1, 0, 0),
        VK_API_VERSION_1_3
    };
    
    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{
        {}, 
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | 
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | 
        vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
        &debugCallback
    };
    
    vk::InstanceCreateInfo createInfo{
        {}, &appInfo,
        static_cast<uint32_t>(enabledLayers.size()), enabledLayers.data(),
        static_cast<uint32_t>(extensions.size()), extensions.data(),
        &debugCreateInfo
    };
    
    try {
        instance = std::make_unique<vk::raii::Instance>(context, createInfo);
        std::cout << "Vulkan instance created with " << enabledLayers.size() << " layer(s)" << std::endl;
    } catch (const vk::LayerNotPresentError& e) {
        std::cerr << "Validation layers not available, creating instance without them" << std::endl;
        createInfo.enabledLayerCount = 0;
        createInfo.ppEnabledLayerNames = nullptr;
        instance = std::make_unique<vk::raii::Instance>(context, createInfo);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to create Vulkan instance: ") + e.what());
    }
}

void VulkanContext::pickPhysicalDevice(const vk::raii::SurfaceKHR& surface) {
    auto devices = instance->enumeratePhysicalDevices();
    if (devices.empty()) {
        throw std::runtime_error("No GPUs with Vulkan support");
    }

    for (const auto& dev : devices) {
        auto queueFamilies = dev.getQueueFamilyProperties();

        for (uint32_t i = 0; i < queueFamilies.size(); i++) {
            if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsFamily = i;
            }
            if (dev.getSurfaceSupportKHR(i, *surface)) {
                presentFamily = i;
            }
        }

        auto extensions = dev.enumerateDeviceExtensionProperties();
        bool hasSwapchain = false;
        for (const auto& ext : extensions) {
            if (strcmp(ext.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
                hasSwapchain = true;
                break;
            }
        }

        if (hasSwapchain) {
            physicalDevice = std::make_unique<vk::raii::PhysicalDevice>(dev);
            return;
        }
    }

    throw std::runtime_error("No suitable GPU found");
}

void VulkanContext::createLogicalDevice() {
    std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily, presentFamily };
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        queueCreateInfos.push_back({ {}, queueFamily, 1, &queuePriority });
    }

    const char* swapchainExt = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    vk::DeviceCreateInfo createInfo{};
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = &swapchainExt;

    device = std::make_unique<vk::raii::Device>(*physicalDevice, createInfo);
    graphicsQueue = std::make_unique<vk::raii::Queue>(*device, graphicsFamily, 0);
    presentQueue = std::make_unique<vk::raii::Queue>(*device, presentFamily, 0);
}
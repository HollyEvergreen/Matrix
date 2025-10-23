#pragma once
#include <vulkan/vulkan_raii.hpp>
#include <SDL3/SDL.h>
#include "Window.h"
#include "Renderer.hpp"

class VulkanContext {
public:
    VulkanContext();
    VulkanContext(SDL_Window* window);
    const vk::raii::Instance& getInstance() const { return *instance; }
    const vk::raii::Device& getDevice() const { return *device; }
    const vk::raii::PhysicalDevice& getPhysicalDevice() const { return *physicalDevice; }
    const vk::raii::Queue& getGraphicsQueue() const { return *graphicsQueue; }
    const vk::raii::Queue& getPresentQueue() const { return *presentQueue; }
    uint32_t getGraphicsQueueFamily() const { return graphicsFamily; }
    uint32_t getPresentQueueFamily() const { return presentFamily; }
    Renderer* renderer;
private:
    void createInstance(SDL_Window* window);
    void pickPhysicalDevice(const vk::raii::SurfaceKHR& surface);
    void createLogicalDevice();

    vk::raii::Context context;
    std::unique_ptr<vk::raii::Instance> instance;
    std::unique_ptr<vk::raii::PhysicalDevice> physicalDevice;
    std::unique_ptr<vk::raii::Device> device;
    std::unique_ptr<vk::raii::Queue> graphicsQueue;
    std::unique_ptr<vk::raii::Queue> presentQueue;
    uint32_t graphicsFamily = 0;
    uint32_t presentFamily = 0;
};
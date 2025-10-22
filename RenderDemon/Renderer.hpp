#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "Window.h"
#include "VulkanContext.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "FrameResources.h"

struct AppCreationContext {
	const char* AppName;
	WindowContext* w_ctx;
};

class Renderer {
public:
    Renderer(AppCreationContext* a_ctx);
    ~Renderer();

    void drawFrame();
    void waitIdle();
    void handleResize();
    void drawMesh();

private:
    Window* m_window;
    const char* AppName;
    SDL_GPUDevice* GPU_Device;
    SDL_Renderer* r_ctx;
    void createSurface();
    void createCommandPool();
    void createFramebuffers();
    void createFrameResources();
    void recordCommandBuffer(vk::CommandBuffer cmd, uint32_t imageIndex);

    // Core components in dependency order
    VulkanContext context;
    std::unique_ptr<vk::raii::SurfaceKHR> surface;
    std::unique_ptr<Swapchain> swapchain;
    Pipeline* pipeline;

    // Command and sync resources
    std::unique_ptr<vk::raii::CommandPool> commandPool;
    std::vector<FrameResources> frameResources;
    std::vector<vk::raii::Framebuffer> framebuffers;

    // Frame management
    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;
    size_t currentFrame = 0;
};
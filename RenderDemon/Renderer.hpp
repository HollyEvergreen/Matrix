#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>
#include "Window.h"
#include "VulkanContext.h"
#include "Swapchain.h"
#include "Pipeline.h"
#include "FrameResources.h"
#include "Mesh/Vertex.hpp"
#include <list>
#include <filesystem>
using Path = std::filesystem::path;

struct AppCreationContext {
	const char* AppName;
	Path HomePath;
	WindowContext* w_ctx;
};

struct Scene {

};

class Renderer {
public:
	Renderer(AppCreationContext* a_ctx);
	~Renderer();

	const Path& GetHomePath() {
		return HomePath;
	}

	void drawFrame();
	void waitIdle();
	void handleResize();
private:
	Window* m_window;
	const char* AppName;
	Path HomePath;
	SDL_GPUDevice* GPU_Device;
	SDL_Renderer* r_ctx;
	int CurrSceneID = 0;

	void createSurface();
	void createCommandPool();
	void createFramebuffers();
	void createFrameResources();
	void createVertexBuffer(const std::vector<Vertex>& vertices);
	void recordCommandBuffer(vk::CommandBuffer cmd, uint32_t imageIndex);
	uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
	
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

	// Scene Resources
	std::vector<Scene> scenes;
};

struct Model {
	using Vertex_Buffer = std::vector<Vertex>;
	using iterator = Vertex_Buffer::iterator;

	Vertex_Buffer* begin() {
		return Meshes.data();
	}
	Vertex_Buffer* end() {
		return Meshes.data() + (Meshes.size() - 1);
	}

private:
	std::vector<Vertex_Buffer> Meshes;
	bool BufIsDirty = true;
	Vertex_Buffer consolidatedBuffer = {};
	const Vertex_Buffer& Consolidate() {
		if (!BufIsDirty) {
			return consolidatedBuffer;
		}
		for (auto& mesh : Meshes) {
			consolidatedBuffer.insert(consolidatedBuffer.end(), mesh.begin(), mesh.end());
		}
		BufIsDirty = false;
		return consolidatedBuffer;
	}
	const Vertex_Buffer& getMesh(int id) {
		return Meshes[id];
	}
	const Vertex& getVertex(int meshId, int vertexId) {
		return Meshes[meshId][vertexId];
	}
	

};

struct Mesh_Data {
	std::vector<vk::Buffer> buffers;
	std::vector<vk::DeviceMemory> memoryUnits;
	static Mesh_Data GenerateMeshData(Model model) {
		for (Model::Vertex_Buffer& vertexBuffer : model) {

		}
	}
};
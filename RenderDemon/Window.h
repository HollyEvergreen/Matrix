#pragma once
#include <SDL3/SDL.h>
#include <array>

struct WindowContext {
	const char* Title;
	std::array<int, 2> dim;
	std::array<float, 4> clearColor;
	SDL_WindowFlags flags;
};

class Window
{
public:
	Window(WindowContext* w_ctx);
	void Update();
	bool ShouldClose();
	SDL_Window* GetHandle() { return m_window; };
	WindowContext* GetContextHandle() { return ctx; }
private:
	SDL_Window* m_window;
	WindowContext* ctx;
};

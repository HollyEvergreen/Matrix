#include "../RenderDemon/Renderer.hpp"
#include "../RenderDemon/WindowFlags.cpp"
#include <iostream>
#include <SDL3/SDL.h>

int main(char** args) {
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD);
	AppCreationContext* a_ctx = new AppCreationContext();
		a_ctx->AppName = "matrix";
		a_ctx->w_ctx = new WindowContext();
			a_ctx->w_ctx->Title = "matrix";
			a_ctx->w_ctx->dim = { 1280, 720 };
			a_ctx->w_ctx->clearColor = { 0, 0, 0 };
			a_ctx->w_ctx->flags = WindowFlags::VULKAN | WindowFlags::BORDERLESS | WindowFlags::FULLSCREEN;

	Renderer* renderer = new Renderer(a_ctx);
	bool running = true;
	while (running) {
		renderer->drawMesh();
		SDL_Event event = SDL_Event();
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_EventType::SDL_EVENT_KEY_DOWN:
				switch (event.key.key)
				{
				case SDLK_ESCAPE:
					running = false;
				default:
					break;
				}
			default:
				break;
			}
		}

		renderer->waitIdle();
		renderer->drawFrame();
	}

	delete renderer;
}
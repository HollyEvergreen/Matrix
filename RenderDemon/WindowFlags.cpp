#include <SDL3/SDL.h>
#include "WindowFlags.hpp"
#define flag(x) x = SDL_WINDOW_##x,

enum WindowFlags {
	flag(FULLSCREEN)
	flag(BORDERLESS)
	flag(RESIZABLE)
	flag(VULKAN)
};


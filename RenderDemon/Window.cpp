#include "Window.h"

Window::Window(WindowContext* w_ctx) {
	m_window = SDL_CreateWindow(w_ctx->Title, w_ctx->dim[0], w_ctx->dim[1], w_ctx->flags);
	this->ctx = w_ctx;
}

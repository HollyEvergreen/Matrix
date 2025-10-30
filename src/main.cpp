#include <iostream>
#include "../MatrixEngine/Matrix.hpp"
#include <bitset>
#include "../RenderDemon/Renderer.hpp"

int main(int argc, char** argv) {
	AppContext::Init({
		{"App-Name", "NBA"},
		{"Version", Version{0, 0, 0}},
		{"Window-Width", 1280},
		{"Window-Height", 720},
		{"Window-flags", std::bitset<8>(0b1)}, // [little endian] 0: vsync, 
		{"Target-FPS", -1}, // -1 refers to render as quickly as possible
	});
	RenderContext::Init({});
	auto renderer = Renderer();
	
	ResetStyling
}

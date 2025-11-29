#include <iostream>
#include <bitset>
//#define MEM_DEBUG
#ifdef MEM_DEBUG
#include <assert.h>
static int Allocations = 0;
static int TotalBytesAllocated = 0;
void* operator new(size_t size) {
	if (size > 2048) {
		__nop();
	}
	void* ptr = malloc(size);
	std::cout << "Allocating " << size << " bytes. This is Allocation " << ++Allocations << ". The Location is " << ptr << ".\n";
	assert(ptr != 0);
	return ptr;
}
void operator delete(void* ptr){
	std::cout << "Deleting " << ptr << "\n";
	return free(ptr);
}
#endif
#include "../MatrixEngine/Matrix.hpp"
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

#pragma once
#include "Shader.hpp"

class Pipeline {
public:
	bool AllShadersCompiled;
	using Shader = int;
	std::vector<Shader> Shaders;
	
	Pipeline() {
		AllShadersCompiled = false;

	}
	void CompileShaders() {
		if (AllShadersCompiled) return;
	};

private:
	void _invalidateShaders(){
		AllShadersCompiled = false;
	}
};
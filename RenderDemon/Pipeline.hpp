#pragma once

class Pipeline {
public:
	bool AllShadersCompiled;
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
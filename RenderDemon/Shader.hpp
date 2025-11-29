#pragma once
#include <vulkan/vulkan.hpp>
#include <Matrix.hpp>

class Shader {
	std::unique_ptr<Matrix::Filesys::File> ShaderFile;
public:
	vk::ShaderModule module;

	Shader() {

	}

};
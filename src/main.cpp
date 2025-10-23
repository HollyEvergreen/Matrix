#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_filesystem.h>
#include <filesystem>
#include <tuple>
#include "../RenderDemon/Renderer.hpp"
#include "../RenderDemon/WindowFlags.cpp"

#pragma region ArgumentParsing
#define BOOLEAN_ARGS \
	BOOLEAN_ARG(help, "-h", "Show help") \
	BOOLEAN_ARG(shaderc, "-s", "Shader Compile") \
	BOOLEAN_ARG(run, "run", "Run NBA")
#include "../easy-args/includes/easyargs.h"
#pragma endregion

/// slow string from C++20 STL
using sstring = std::string;
using Path = std::filesystem::path;

static __forceinline Path ProjectSourcePath() {
	return Path(__src_dir__);
}

struct shaderDir {
	const uint8_t magicbyte = 0x69;
	sstring outputFolder;
	std::vector<std::array<sstring, 2>> shader_files;
};

static SDL_EnumerationResult directory_enumeration_callback(void* userdata, const char* _dirname, const char* _fname){
	switch (((uint8_t*)userdata)[0])
	{
	case 0x69:
		{
		shaderDir * dir = (shaderDir*)userdata;
		sstring fname = _fname;
		sstring dirname = _dirname;
		if (fname == "out") {
			dir->outputFolder = dirname + fname;
		}
		else if (fname == "shaders.meta");
		else {
			auto file = Path(fname);
			dir->shader_files.push_back({ dirname, file.filename().string()});
		}
		return SDL_ENUM_CONTINUE;
		}
	default:
		return SDL_ENUM_CONTINUE;
		break;
	}
}

static void EnumerateShaderDir(shaderDir& dir) {
	auto path = ProjectSourcePath(); 
	path += "/res/Shaders";
	SDL_EnumerateDirectory(path.string().c_str(), (SDL_EnumerateDirectoryCallback)directory_enumeration_callback, &dir);
}

int main(int arg_count, char** argv) {
	static auto home_path = Path(SDL_GetPrefPath("", "NBA"));
	args_t args = make_default_args();
	if (!parse_args(arg_count, argv, &args) || args.help) {
		print_help(argv[0]);
		return 1;
	}
	bool run = false;
	bool shaderc = false;
#define DEBUG__RUN__
#ifdef DEBUG__RUN__
	run = true;
#else
	shaderc = true;
#endif // SHADERC

	if (args.run or run) {
		SDL_Init(SDL_INIT_AUDIO | SDL_INIT_GAMEPAD);
		AppCreationContext* a_ctx = new AppCreationContext();
		a_ctx->AppName = "matrix";
		a_ctx->HomePath = home_path;
		a_ctx->w_ctx = new WindowContext();
		a_ctx->w_ctx->Title = "matrix";
		a_ctx->w_ctx->dim = { 1280, 720 };
		a_ctx->w_ctx->clearColor = { 0, 0, 0 };
		a_ctx->w_ctx->flags = WindowFlags::VULKAN | WindowFlags::BORDERLESS | WindowFlags::FULLSCREEN;

		Renderer* renderer = new Renderer(a_ctx);
		bool running = true;
		while (running) {
			SDL_Event event = SDL_Event();
			while (SDL_PollEvent(&event)) {
				switch (event.type)
				{
				case SDL_EventType::SDL_EVENT_KEY_DOWN:
					switch (event.key.key)
					{
					case SDLK_ESCAPE:
						running = false;
						break;
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
	else if (args.shaderc or shaderc) {
		shaderDir paths;
		EnumerateShaderDir(paths);
		std::cout << "output path: " << paths.outputFolder << "\n";
		auto meta_file = fopen((paths.shader_files[0][0] + "\\shaders.meta").c_str(), "w");
		sstring shaderPaths;

		for (auto& shader_file : paths.shader_files){
			sstring cmd;
			auto shader_file_path = Path(shader_file[1]);
			if (shader_file_path.extension().string() == ".hlsl") {
				// x.type.hlsl -> x.type.spv
				auto ext = shader_file_path.replace_extension().extension().string();
				sstring type;
				if (ext == ".frag") {
					type = "ps_6_0";
				}
				else if (ext == ".vert") {
					type = "vs_6_0";
				}
				else {
					throw std::runtime_error(ext);
				}

				const auto out_shader_file = Path(shader_file[1]).replace_extension();
				cmd = std::format(
					"dxc -spirv -T {0} -E main {1}{2} -Fo {4}\\{3}",
					type,
					shader_file[0],
					shader_file[1],
					Path(shader_file[1]).replace_extension(".spv").string(),
					paths.outputFolder
				);
				std::cout << cmd <<"\n";
			}
			else {
				cmd = std::format(
					//x.type.glsl -> x.type -> x.type.spv
					"glslangValidator -V {0}{1} -o {2}\\{1}.spv",
					shader_file[0], 
					shader_file[1], 
					paths.outputFolder
				);
			}
			system(cmd.c_str());
			shaderPaths += std::format("{0}Shaders\\out\\{1}\n", home_path.string(), Path(shader_file[1]).replace_extension(".spv").string());
		}
		fprintf(meta_file, shaderPaths.c_str());
		fclose(meta_file);
		auto cmd = std::format("cmake -E copy_directory \"{0}\" \"{1}Shaders\"", paths.shader_files[0][0].substr(0, paths.shader_files[0][0].length()-1), home_path.string());
		std::cout << cmd << "\n";
		system(cmd.c_str());
	}
}
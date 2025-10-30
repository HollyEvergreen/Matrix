include_directories("C:/VulkanSDK/1.4.313.2/Include")

project("RenderDemon")

set(
	imgui_sources
	"imgui/imgui.cpp"
	"imgui/imgui.h"
	"imgui/imgui_demo.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_internal.h"
	"imgui/imgui_tables.cpp"
	"imgui/imgui_widgets.cpp" 
	"imgui/imstb_rectpack.h"
	"imgui/imstb_textedit.h"
	"imgui/imstb_truetype.h"
)

set(
	imgui_backends
	"imgui/backends/imgui_impl_vulkan.cpp"
	"imgui/backends/imgui_impl_vulkan.h"
	"imgui/backends/imgui_impl_win32.cpp"
	"imgui/backends/imgui_impl_win32.h"
	"imgui/backends/imgui_impl_glfw.cpp"
	"imgui/backends/imgui_impl_glfw.h"
)

add_library(${PROJECT_NAME} ${imgui_sources} ${imgui_backends} "Renderer.hpp")
find_package(Vulkan REQUIRED)
target_link_libraries(RenderDemon Vulkan::Vulkan)

add_subdirectory("glfw")
target_link_libraries(${PROJECT_NAME} glfw "MatrixEngine")
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
)
target_include_directories(${PROJECT_NAME} PUBLIC "./glfw/include/" "./imgui/" "./imgui/backends" "../MatrixEngine/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RenderDemon PROPERTY CXX_STANDARD 20)
endif()

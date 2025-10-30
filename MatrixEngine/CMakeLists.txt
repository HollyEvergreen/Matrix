# CMakeList.txt : CMake project for MatrixEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
project("MatrixEngine")
add_library (${PROJECT_NAME} "FileSystem.hpp" "FileSystem.cpp" "DateTime.cpp"  "AppContext.hpp"  "AppContext.cpp" "Defines.h" "RenderContext.cpp")

target_link_libraries(MatrixEngine "RenderDemon" "glfw")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# TODO: Add tests and install targets if needed.

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Matrix")

# Include sub-projects.
add_subdirectory ("MatrixEngine")
add_subdirectory ("RenderDemon")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(Matrix "src/main.cpp")

target_link_libraries(MatrixEngine)
target_link_libraries(RenderDemon)
target_link_libraries(Matrix PRIVATE SDL3::SDL3)

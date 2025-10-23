find_package(Vulkan REQUIRED)

include_directories("C:/VulkanSDK/1.4.313.2/Include")

add_library(RenderDemon "blank.cpp")
target_link_libraries(RenderDemon Vulkan::Vulkan)

include_directories ("D:/VS2022/repos/Matrix/vendored/SDL/include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RenderDemon PROPERTY CXX_STANDARD 20)
endif()

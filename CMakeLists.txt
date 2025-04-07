cmake_minimum_required(VERSION 3.16)

project(opengl_animation_transitions VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
add_subdirectory(libs/imgui_wrapper)
add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
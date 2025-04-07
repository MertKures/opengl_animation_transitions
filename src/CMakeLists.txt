find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} animation_transitions/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad imgui)
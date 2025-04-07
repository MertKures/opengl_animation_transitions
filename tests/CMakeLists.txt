# == OpenGL + Glad + GLFW

find_package(OpenGL REQUIRED)

add_executable(glad_test
    glad.test.cpp
)

target_link_libraries(glad_test PRIVATE glad glfw OpenGL::GL)

add_test(NAME glad_test COMMAND glad_test)
set_tests_properties(glad_test PROPERTIES LABELS "opengl")


# == ImGui ==

add_executable(imgui_test
    imgui.test.cpp
)

target_link_libraries(imgui_test PRIVATE glad glfw imgui OpenGL::GL)

add_test(NAME imgui_test COMMAND imgui_test)
set_tests_properties(imgui_test PROPERTIES LABELS "imgui")
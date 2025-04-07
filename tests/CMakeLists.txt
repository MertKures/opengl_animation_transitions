# == OpenGL + Glad + GLFW

find_package(OpenGL REQUIRED)

add_executable(glad_test
    glad.test.cpp
)

target_link_libraries(glad_test PRIVATE glad glfw OpenGL::GL)

add_test(NAME glad_test COMMAND glad_test)
set_tests_properties(glad_test PROPERTIES LABELS "opengl")
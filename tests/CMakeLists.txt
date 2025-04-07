add_executable(${PROJECT_NAME}_tests
    glad.test.cpp
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE glad glfw OpenGL::GL)

add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)
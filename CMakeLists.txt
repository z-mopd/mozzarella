cmake_minimum_required(VERSION 4.1.2)

project("opengl-app")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)

add_subdirectory(src)

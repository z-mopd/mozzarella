cmake_minimum_required(VERSION 4.1.2)

project(mzr)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakeOptions.txt)


set(pubinclude "${CMAKE_SOURCE_DIR}/include")
set(mzrinclude "${pubinclude}/mzr")

find_package(OpenGL REQUIRED)

add_subdirectory(src mozzarella)

if (BUILD_TESTS)
   add_subdirectory(tests)
endif()

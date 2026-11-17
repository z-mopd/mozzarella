cmake_minimum_required(VERSION 4.1.2)

file(READ "VERSION.semver" verfile)
string(REGEX MATCH "[0-9]+\.[0-9]+.[0-9]+" ver ${verfile})

project(mzr VERSION ${ver})
message("Configuring Mozzarella version ${CMAKE_PROJECT_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

include(CMakeOptions.txt)

if (${COMPILE_COMMANDS})
   set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

set(pubinclude "${CMAKE_SOURCE_DIR}/include")
set(mzrinclude "${pubinclude}/mzr")

add_subdirectory(src mozzarella)

if (${BUILD_TESTS})
   add_subdirectory(tests)
endif()

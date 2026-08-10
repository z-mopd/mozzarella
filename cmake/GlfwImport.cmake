add_subdirectory(external/glfw)

list(APPEND mzrlib_sources $<TARGET_OBJECTS:glfw>)
include_directories(BEFORE SYSTEM external/glfw/include)

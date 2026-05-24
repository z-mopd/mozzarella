add_executable(main
   main.cpp
   vertex.cpp
   render.cpp
   glad.c
)

target_link_libraries(main PRIVATE glfw)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
find_package(OpenGL REQUIRED)

target_link_libraries(main PRIVATE OpenGL::GL)

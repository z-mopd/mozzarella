set(mzrlib_sources
   core.cpp
   window.cpp
   shape.cpp
   color.cpp
)

string(TOUPPER ${BACKEND} BACKEND_INSENSITIVE)

if (${BACKEND_INSENSITIVE} STREQUAL "OPENGL")
   add_compile_definitions(MZR_OPENGL)
   find_package(OpenGL REQUIRED)

   set(PLATFORM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/platform/opengl")

   list(
      APPEND mzrlib_sources
      ${PLATFORM_PATH}/api_gl.c
      ${PLATFORM_PATH}/glad.c
      ${PLATFORM_PATH}/glsl_sources.c
      ${PLATFORM_PATH}/render_gl.c
   )
elseif(${BACKEND_INSENSITIVE} == "VULKAN")
   add_compile_definitions(MZR_VULKAN)
else()
   message(FATAL_ERROR "Mozzarella is not supported on this platform!")
endif()

include(GlfwImport)

add_library(mzrlib
   STATIC
   ${mzrlib_sources}
)

target_link_libraries(mzrlib PRIVATE OpenGL::GL)
target_include_directories(mzrlib 
   PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/platform
   ${mzrinclude}

   PUBLIC
   ${pubinclude}
)

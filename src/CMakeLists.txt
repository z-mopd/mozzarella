set(mzrlib_sources
   external/glad.c
   _vertex.cpp
   _render.cpp
   core.cpp
   window.cpp
   shape.cpp
   color.cpp
)

include(GlfwImport)

add_library(mzrlib
   STATIC
   ${mzrlib_sources}
)

target_link_libraries(mzrlib PRIVATE OpenGL::GL)
target_include_directories(mzrlib 
   PRIVATE  ${CMAKE_CURRENT_BINARY_DIR} ${mzrinclude}
   PUBLIC   ${pubinclude}
)

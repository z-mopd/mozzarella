#ifndef GL_DEF_H
#define GL_DEF_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include "../api.h"

// glsl_sources
extern const char* shape2DVert;
extern const char* shape2DFrag;
extern const char* textVert;
extern const char* textFrag;


// api definitions
struct __mzr_shader {
   GLuint program;
};

typedef struct __mzr_shader glshader;

struct __mzr_window {
   GLFWwindow* underlying;
   GLuint immediate_vao;
};

struct __mzr_buffer {
   size_t size;
   GLuint vbo;
};

struct __mzr_api_ctx {
   glshader shader2d;
   glshader shaderText;

   GLFWwindow* first_window;

   GLuint current_program;
   GLuint immediate_vbo;
};

#endif

#include <stdio.h>
#include <stdlib.h>
#include "gl_def.h"

#include <GLFW/glfw3.h>

/* Some important things to note:
 * VBOs are shared across OpenGL contexts. VAOs are not.
 * Creating a first window with no width or height is invalid
*/

GLuint gl_create_shader_program(const char** vert, const char** frag) {
   GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vShader, 1, vert, NULL);
   glCompileShader(vShader);

   GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fShader, 1, frag, NULL);
   glCompileShader(fShader);

   GLuint program = glCreateProgram();

   glAttachShader(program, vShader);
   glAttachShader(program, fShader);
   glLinkProgram(program);

   glDeleteShader(vShader);
   glDeleteShader(fShader);

   return program;
}

// api lifecycle
MzrApiCtx api_ctx(void) {
   return (MzrApiCtx)malloc(sizeof(struct __mzr_api_ctx));
}

int api_init(MzrApiCtx ctx, double* initTime) {
   if (!glfwInit()) {
      fprintf(stderr, "Could not start glfw!\n");
      return AGNOSTIC_GLFW_FAIL;
   }

   glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

   GLFWwindow* window = glfwCreateWindow(10, 10, "", NULL, NULL);
   if (!window) {
      glfwTerminate();
      return AGNOSTIC_GLFW_FAIL;
   }
   glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
   ctx->first_window = window;

   glfwMakeContextCurrent(window);

   // make sure to use gladLoadGL AFTER a valid OpenGL context has been made and current
   // failing to do will lead to a segmentation fault
   gladLoadGL();

   glViewport(0, 0, 0, 0);

   ctx->shader2d.program   = gl_create_shader_program(&shape2DVert, &shape2DFrag);
   glUseProgram(ctx->shader2d.program);

   ctx->shaderText.program = gl_create_shader_program(&textVert, &textFrag);

   // generate buffers for immediate drawing
   glGenBuffers(1, &ctx->immediate_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, ctx->immediate_vbo);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float)*AGNOSTIC_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   *initTime = glfwGetTime();

   return AGNOSTIC_SUCCESS;
}

int api_done(MzrApiCtx ctx) {
   glfwDestroyWindow(ctx->first_window);
   glfwMakeContextCurrent(NULL);
   glfwTerminate();

   free(ctx);
   return AGNOSTIC_SUCCESS;
}

MzrWindow api_create_window(
   MzrApiCtx ctx,
   const char* title,
   size_t x, size_t y,
   size_t width, size_t height
) {
   MzrWindow window = (MzrWindow)malloc(sizeof(struct __mzr_window));
   GLFWwindow* underlying = glfwCreateWindow(width, height, title, NULL, ctx->first_window);
   window->underlying = underlying;

   glfwMakeContextCurrent(underlying);

   glGenVertexArrays(1, &window->immediate_vao);

   glBindVertexArray(window->immediate_vao);
   glBindBuffer(GL_ARRAY_BUFFER, ctx->immediate_vbo);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return window;
}

void api_done_window(MzrApiCtx ctx, MzrWindow window) {
   glfwSetWindowShouldClose(window->underlying, GLFW_TRUE); // Just in case
   glfwDestroyWindow(window->underlying);
   window->underlying = NULL;
   free(window);
}

void api_prepareFrame_window(MzrApiCtx ctx, MzrWindow window) {
   glBindVertexArray(window->immediate_vao);
}

GLFWwindow* getGlfw(MzrWindow window) {
   return window->underlying;
}

MzrShader api_shaderNull(MzrApiCtx ctx) {
   MzrShader nullShader = malloc(sizeof(struct __mzr_shader));
   nullShader->program = 0;
   return nullShader;
}

MzrShader api_shader2d(MzrApiCtx ctx) {
   return &ctx->shader2d;
}

MzrShader api_shaderText(MzrApiCtx ctx) {
   return &ctx->shaderText;
}

int api_bind_shader(MzrApiCtx ctx, MzrShader shader) {
   glUseProgram(shader->program);
   ctx->current_program = shader->program;

   return AGNOSTIC_SUCCESS;
}

int api_is_shader_bound(MzrApiCtx ctx, MzrShader shader) {
   return ctx->current_program == shader->program;
}

MzrTexture api_texture(TextureType type) {

}

MzrBuffer mzrgl_alloc_buffer(size_t size, GLenum usage) {
   MzrBuffer buffer = (MzrBuffer)malloc(sizeof(struct __mzr_buffer));
   buffer->size = size;

   glGenBuffers(1, &(buffer->vbo));

   glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
   glBufferData(GL_ARRAY_BUFFER, size, NULL, usage);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   return buffer;
}

MzrBuffer api_alloc_dynamic_buffer(
   MzrApiCtx ctx,
   size_t vertECount,
   size_t vertESize,
   size_t vertStride,
   size_t vertCount
) {
   return mzrgl_alloc_buffer(vertECount*vertESize*vertCount, GL_DYNAMIC_DRAW);
}

MzrBuffer api_alloc_static_buffer(
   MzrApiCtx ctx,
   size_t vertECount,
   size_t vertESize,
   size_t vertStride,
   size_t vertCount
) {
   return mzrgl_alloc_buffer(vertECount*vertESize*vertCount, GL_STATIC_DRAW);
}

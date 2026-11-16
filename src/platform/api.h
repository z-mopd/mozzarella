// api.h
// General, abstract interface for the OpenGL and Vulkan backends to implement
// It is meant to decouple API-specific details from the logic of the overall rendering library (which can get very complex at times)
// Public consumers of Mozzarella shouldn't worry their sweet little minds over api.h; it is intended for internal usage only
//
// API selection is done at compilation

#ifndef API_H
#define API_H

#define ISNULL_BACKEND 0x00000000
#define OPENGL_BACKEND 0x00000010
#define VULKAN_BACKEND 0x00000100

#define METAL_BACKEND  ISNULL_BACKEND
#define DIRCTX_BACKEND ISNULL_BACKEND

#ifdef MZR_OPENGL
#  define CURRENT_BACKEND OPENGL_BACKEND
#elif defined(MZR_VULKAN)
#  define CURRENT_BACKEND VULKAN_BACKEND
#else
#  define CURRENT_BACKEND ISNULL_BACKEND
#endif

#define AGNOSTIC_BUFFER_SIZE   8192

#define AGNOSTIC_UNKNOWN       -1
#define AGNOSTIC_SUCCESS        0
//#define AGNOSTIC_FREETYPE_FAIL  1
#define AGNOSTIC_GLFW_FAIL      2

#ifdef __cplusplus
// I hate C++.
extern "C" {
#endif
#include <GLFW/glfw3.h>

struct __mzr_api_ctx;
typedef struct __mzr_api_ctx* MzrApiCtx;

struct __mzr_window;
typedef struct __mzr_window* MzrWindow;

struct __mzr_buffer;
typedef struct __mzr_buffer* MzrBuffer;

struct __mzr_shader;
typedef struct __mzr_shader* MzrShader;

struct __mzr_texture;
typedef struct __mzr_texture* MzrTexture;

typedef enum {
   Texture1D,
   Texture2D,
   Texture3D
} TextureType;

MzrApiCtx api_ctx(void); // allocates memory on the heap
int api_init(MzrApiCtx ctx, double* initTime);
int api_done(MzrApiCtx ctx); // frees memory

// Creates a window with the specified position and size
MzrWindow api_create_window(MzrApiCtx ctx, const char* title, size_t x, size_t y, size_t width, size_t height);
void api_done_window(MzrApiCtx ctx, MzrWindow window);

void api_prepareFrame_window(MzrApiCtx ctx, MzrWindow window);

GLFWwindow* getGlfw(MzrWindow window);

MzrShader api_shaderNull(MzrApiCtx ctx);
MzrShader api_shader2d(MzrApiCtx ctx);
MzrShader api_shaderText(MzrApiCtx ctx);

// TODO: Pipelines?
int api_bind_shader(MzrApiCtx ctx, MzrShader shader);
int api_is_shader_bound(MzrApiCtx ctx, MzrShader shader);

MzrTexture api_texture(TextureType type);

// Note, in the future, something similar to Vulkan & OpenGL UBOs (Uniform Buffer Object) should be implemented to replace this approach
void api_set_projection(MzrApiCtx ctx, float* mat4x4);

// set_color
// Expects all values to be normalized in the range [0, 1]
void api_set_color(MzrApiCtx ctx, float r, float g, float b, float a);

MzrBuffer api_alloc_dynamic_buffer(
   MzrApiCtx ctx,
   size_t vertAttribElementCount, // how many elements a vertex attribute has
   size_t vertAttribElementSize,  // byte size of a vertex element in a vertex attribute
   size_t vertAttribStride, // byte offset between consecutive attributes
   size_t vertAttribCount // actual number of vertex attributes
);

MzrBuffer api_alloc_static_buffer(
   MzrApiCtx ctx,
   size_t vertAttribElementCount,
   size_t vertAttribElementSize,
   size_t vertAttribStride,
   size_t vertAttribCount
);

// submit_triangle
// Pushes a triangle draw call onto the stack
void api_submit_triangle(
   MzrApiCtx ctx,
   float x1, float y1,
   float x2, float y2,
   float x3, float y3
);

// draw_triangle
// Immediately draws a triangle without reserving a call on the stack
void api_draw_triangle(
   MzrApiCtx ctx,
   float x1, float y1,
   float x2, float y2,
   float x3, float y3
);

// write_triangle
// Writes the vertex data of a triangle to a buffer for later use
// returns size_t: how many bytes written
size_t api_write_triangle(
   MzrApiCtx ctx,
   MzrBuffer buffer,
   size_t byteOffset,
   float x1, float y1,
   float x2, float y2,
   float x3, float y3
);

// draw_triangle_buffer
// Similar to draw_triangle but buffered
// Notice to future self: don't forget that you can use this to implement batched drawing ;>
void api_draw_triangle_buffer(MzrApiCtx ctx, MzrBuffer buffer, size_t triangleCount);

void api_draw_rectangle(MzrApiCtx ctx, float x, float y, float width, float height);

void api_draw_circle(MzrApiCtx ctx, float* vertex_buffer, float xpos, float ypos, float rad, float segments);

void api_draw_line(
   MzrApiCtx ctx,
   float x1, float y1,
   float x2, float y2
);

// write_line
// Writes the vertex data of a line to a buffer for later use
// returns size_t: how many bytes written
size_t api_write_line(
   MzrApiCtx ctx,
   MzrBuffer buffer,
   float x1, float y1,
   float x2, float y2
);

#ifdef __cplusplus
// With a burning passion!
} // extern "C"
#endif

#endif // API_H
// Yet, I still used C++ for Mozzarella.
// This is the only C in the library! (Save for the external C libraries)

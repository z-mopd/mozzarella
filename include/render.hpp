#ifndef RENDER_HPP
#define RENDER_HPP

#include "vertex.hpp"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <cstddef>

struct thread_t {};

typedef struct {
   int2 size;
   thread_t* _thread;
   GLFWwindow* _underlying;
} Window;

void r_init();
void r_close();

void window_hints             (size_t count, ...);
void window_destroy           (Window* window);
void window_con_absolute      (Window* window, int2 pos, int2 size, const char* title, thread_t* thread);
void window_con_relative      (Window* window, float2 pos, float2 size, const char* title, thread_t* thread);
void window_wait_for_close    (Window* window);

#endif

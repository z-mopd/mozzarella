#include "render.hpp"
#include "GLFW/glfw3.h"

#include <GL/gl.h>
#include <cstdarg>
#include <iostream>
#include <thread>

static GLFWwindow* first_context = nullptr;

void r_init() {
   if (!glfwInit()) {
      std::cout << "Could not start glfw\n";
      return;
   }

   glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
   first_context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
   if (!first_context) {
      glfwTerminate();
      return;
   }

   glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

   glfwMakeContextCurrent(first_context);
   gladLoadGL();
   glViewport(0, 0, 1, 1);
}

void r_close() {
   glfwDestroyWindow(first_context);
   glfwTerminate();
}

void window_hints(size_t count, ...) {
   va_list args;
   va_start(args, count);

   for (size_t i = 0; i < count; ++i) {
      glfwWindowHint(va_arg(args, int), va_arg(args, int));
   }

   va_end(args);
}

void window_destroy(Window* window) {
   glfwDestroyWindow(window->_underlying);
   window->_underlying = nullptr;
}

void window_con_absolute(Window* window, int2 pos, int2 size, const char* title, thread_t* thread) {
   window->size = size;

   GLFWwindow* _underlying;
   _underlying = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
   window->_underlying = _underlying;

   glfwMakeContextCurrent(_underlying);
   glViewport(pos.x, pos.y, size.x, size.y);

   std::thread window_thread{[_underlying,window]{
      while (!glfwWindowShouldClose(_underlying)) {
         glfwSwapBuffers(_underlying);
         glfwPollEvents();
      }

      window_destroy(window);
   }};

   window_thread.join();
}


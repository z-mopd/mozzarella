#include "_common.hpp"
#include "_helper.hpp"
#include "_vertex.hpp"
#include "_render.hpp"
#include "window.hpp"
#include "input.hpp"
#include "shape.hpp"
#include <GLFW/glfw3.h>
#include <vector>

void noop(mzr::Window* window, mzr::MouseButton button, mzr::Action action) {}

namespace mzr {

Window::Window(int2 pos, int2 size, const char* title, std::initializer_list<int> hints_values) : pos(pos), size(size) {
   if (hints_values.size() > 0) {
      const int* start = hints_values.begin();
      for (size_t i = 0; i < hints_values.size(); ++i) {
         glfwWindowHint(start[i], 1);
      }
   }

   this->_underlying = glfwCreateWindow(size.x, size.y, title, nullptr, first_context());
   GLFWwindow* underlying = static_cast<GLFWwindow*>(this->_underlying);

   glfwSetWindowUserPointer(underlying, this);
   glfwSetInputMode(underlying, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);

   this->_callback = noop;
   glfwSetMouseButtonCallback(underlying, [](GLFWwindow* window, int button, int action, int mods) {
      auto casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
      casted_window->_callback(casted_window, static_cast<MouseButton>(button), glfwEnum_to_action(action));
   });

   glfwMakeContextCurrent(underlying); // everything before this should not rely on the current context
   glViewport(pos.x, pos.y, size.x, size.y);
}

Window::Window(int2 pos, int2 size, const char* title) : Window(pos, size, title, {}) {}
Window::Window(int x, int y, int width, int height, const char* title, std::initializer_list<int> hints) : Window({x, y}, {width, height}, title, hints) {}
Window::Window(int x, int y, int width, int height, const char* title) : Window({x, y}, {width, height}, title, {}) {}

void Window::Close() {
   glfwDestroyWindow(static_cast<GLFWwindow*>(this->_underlying));
   this->_underlying = nullptr;
   glfwMakeContextCurrent(nullptr);
}

bool Window::ShouldClose() {
   return glfwWindowShouldClose(static_cast<GLFWwindow*>(this->_underlying));
}

void Window::SetVisible(bool value) {
   if (value)
      glfwShowWindow(static_cast<GLFWwindow*>(this->_underlying));
   else
      glfwHideWindow(static_cast<GLFWwindow*>(this->_underlying));
}

void Window::MakeCurrent() {
   glfwMakeContextCurrent(static_cast<GLFWwindow*>(this->_underlying));
}

void Window::MouseCallback( void(*callback)(Window* window, MouseButton button, Action action) ) {
   this->_callback = callback;
}

float2 Window::GetMousePos() {
   double x, y;
   glfwGetCursorPos(static_cast<GLFWwindow*>(this->_underlying), &x, &y);
   return float2{static_cast<float>(x), static_cast<float>(y)};
}

bool Window::IsMousePressed(MouseButton button) {
   Action current_state = glfwEnum_to_action(glfwGetMouseButton(static_cast<GLFWwindow*>(this->_underlying), button));

   if (_prev_state != Action::PRESS && current_state == Action::PRESS) {
      _prev_state = Action::PRESS;
      return true;
   }
   else {
      _prev_state = current_state;
      return false;
   }
}

double Window::GetFrameTime() {
   return this->_dt;
}

void Window::BeginDrawing() {
   glfwMakeContextCurrent(static_cast<GLFWwindow*>(this->_underlying));
   glUseProgram(query_shader_program());
   this->_last_marked = glfwGetTime();
}

void Window::Clear(Color color) {
   glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a);
   glClear(GL_COLOR_BUFFER_BIT);
}


// Batching

// TODO:
// * make batch drawing work with the abstract Shape class
// * find some other way to batch vertices rather than relying on vector
void Window::BatchDraw(Triangle* triangles, std::size_t n, Color color) {
   std::size_t count = 0;
   std::vector<float> geometry;

   for (std::size_t i = 0; i < n; ++i) {
      count += (triangles + i)->_count();
   }
   geometry.resize(buffer_nsize(count));

   std::size_t offset = 0;
   for (std::size_t i = 0; i < n; ++i) {
      (triangles + i)->GenerateVertices(geometry.data() + offset, this->size, color);
      offset += buffer_nsize((triangles + i)->_count());
   }

   submit_immediate_call(GL_TRIANGLES, geometry.data(), buffer_byte_size(float, count), count);
   update_vertex_attrib();
   draw();
}

void Window::BatchDraw(Rectangle* rectangles, std::size_t n, Color color) {
   std::size_t count = 0;
   std::vector<float> geometry;

   for (std::size_t i = 0; i < n; ++i) {
      count += (rectangles + i)->_count();
   }
   geometry.resize(buffer_nsize(count));

   std::size_t offset = 0;
   for (std::size_t i = 0; i < n; ++i) {
      (rectangles + i)->GenerateVertices(geometry.data() + offset, this->size, color);
      offset += buffer_nsize((rectangles + i)->_count());
   }

   submit_immediate_call(GL_TRIANGLES, geometry.data(), buffer_byte_size(float, count), count);
   update_vertex_attrib();
   draw();
}

void Window::BatchDraw(Circle* circles, std::size_t n, Color color) {}


// Static draw functions

// TODO:
// * another way to implement static drawables
// * this isn't elegant or performant enough
template<>
void Window::Draw<Triangle>(StaticDrawable<Triangle> triangle) {
   if (_static_vbos.find(triangle._id) != _static_vbos.end()) {
      draw_static(GL_TRIANGLES, _static_vbos[triangle._id], 3);
   }
   else {
      alloc_shape_buffer(float, vertices, 3);
      triangle._shape.GenerateVertices(vertices, this->size, triangle._color);

      GLuint vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, buffer_byte_size(float, 3), vertices, GL_STATIC_DRAW);

      _static_vbos[triangle._id] = vbo;
      draw_static(GL_TRIANGLES, vbo, 3);
   }
}
template<>
void Window::Draw<Rectangle>(StaticDrawable<Rectangle> rectangle) {
   if (_static_vbos.find(rectangle._id) != _static_vbos.end()) {
      draw_static(GL_TRIANGLES, _static_vbos[rectangle._id], 6);
   }
   else {
      alloc_shape_buffer(float, vertices, 6);
      rectangle._shape.GenerateVertices(vertices, this->size, rectangle._color);

      GLuint vbo;
      glGenBuffers(1, &vbo);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, buffer_byte_size(float, 6), vertices, GL_STATIC_DRAW);

      _static_vbos[rectangle._id] = vbo;
      draw_static(GL_TRIANGLES, vbo, 6);
   }
}
template<>
void Window::Draw<Circle>(StaticDrawable<Circle>) {}


// Regular draw functions

void Window::Draw(Triangle triangle, Color color) {
   alloc_shape_buffer(float, vertices, 3);
   triangle.GenerateVertices(vertices, this->size, color);
   submit_immediate_call(GL_TRIANGLES, vertices, buffer_byte_size(float, 3), 3);
   update_vertex_attrib();
   draw();
}

void Window::Draw(Rectangle rectangle, Color color) {
   alloc_shape_buffer(float, vertices, 6);
   rectangle.GenerateVertices(vertices, this->size, color);
   submit_immediate_call(GL_TRIANGLES, vertices, buffer_byte_size(float, 6), 6);
   update_vertex_attrib();
   draw();
}

void Window::Draw(Circle circle, Color color) {
   //TODO: work this out
   float2 adjusted_pos = project_point_to_screen(circle.pos, this->size);
   float adjusted_size = circle.radius;
}

void Window::FlushRender() {
   flush();
}


void Window::EndDrawing() {
   this->_dt = glfwGetTime() - this->_last_marked;
   glfwSwapBuffers(static_cast<GLFWwindow*>(this->_underlying));
}

}

#include "_common.hpp"
#include "_helper.hpp"
#include "_render.hpp"
#include "_vertex.hpp"
#include "window.hpp"
#include "input.hpp"
#include "shape.hpp"

void noop(mzr::Window* window, MouseButton button, Action action) {}

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

void Window::Close() {
   glfwDestroyWindow(static_cast<GLFWwindow*>(this->_underlying));
   this->_underlying = nullptr;
   glfwMakeContextCurrent(nullptr);
}

bool Window::ShouldClose() {
   return glfwWindowShouldClose(static_cast<GLFWwindow*>(this->_underlying));
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

void Window::Draw(Triangle triangle, Color color) {
   GLfloat2 win_sizef = {static_cast<GLfloat>(this->size.x), static_cast<GLfloat>(this->size.y)};

   GLfloat2 adjusted_p1 = project_point_to_screen(f2_to_glf2(triangle.p1), win_sizef);
   GLfloat2 adjusted_p2 = project_point_to_screen(f2_to_glf2(triangle.p2), win_sizef);
   GLfloat2 adjusted_p3 = project_point_to_screen(f2_to_glf2(triangle.p3), win_sizef);

   alloc_shape_buffer(GLfloat, vertices, 3);
   gen_triangle_direct(vertices, 3, adjusted_p1, adjusted_p2, adjusted_p3);
   map_color_ntimes(vertices + 9, 3, color.normalized());

   glBindVertexArray(triangle.get_sid());
   glBindBuffer(GL_ARRAY_BUFFER, triangle.get_bid());
   glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size(GLfloat, 3), vertices);
   glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Window::Draw(Rectangle rectangle, Color color) {
   GLfloat2 win_sizef = {static_cast<GLfloat>(this->size.x), static_cast<GLfloat>(this->size.y)};

   GLfloat2 adjusted_pos = project_point_to_screen(f2_to_glf2(rectangle.pos), win_sizef);
   GLfloat2 adjusted_size = {(rectangle.size.x*2)/win_sizef.x, (rectangle.size.y*2)/win_sizef.y};

   alloc_shape_buffer(GLfloat, vertices, 6);
   gen_rectangle_vert(vertices, 3, adjusted_pos, adjusted_size);
   map_color_ntimes(vertices + 18, 6, color.normalized());

   glBindVertexArray(rectangle.get_sid());
   glBindBuffer(GL_ARRAY_BUFFER, rectangle.get_bid());
   glBufferSubData(GL_ARRAY_BUFFER, 0, buffer_size(GLfloat, 6), vertices);
   glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Window::Draw(Circle circle, Color color) {
   //TODO: implement this
}

void Window::EndDrawing() {
   this->_dt = glfwGetTime() - this->_last_marked;
   glfwSwapBuffers(static_cast<GLFWwindow*>(this->_underlying));
}

}

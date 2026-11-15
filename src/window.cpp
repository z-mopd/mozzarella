#include "_common.hpp"
#include "_helper.hpp"
#include "core.hpp"
#include "platform/api.h"

#include "window.hpp"

#include <GLFW/glfw3.h>

void createOrthographic(float* buffer, float left, float right, float bottom, float top, float near, float far) {
   const float sx = 2 / (right - left);
   const float sy = 2 / (top - bottom);
   const float sz = 2 / (near - far);
   const float tx = -(right + left) / (right - left);
   const float ty = -(top + bottom) / (top - bottom);
   const float tz = -(far + near) / (near - far);


   buffer[0] = sx;  buffer[1] = 0;   buffer[2] = 0;   buffer[3] = 0;

   buffer[4] = 0;   buffer[5] = sy;  buffer[6] = 0;   buffer[7] = 0;

   buffer[8] = 0;   buffer[9] = 0;   buffer[10] = sz; buffer[11] = 0;

   buffer[12] = tx; buffer[13] = ty; buffer[14] = tz; buffer[15] = 1;
}

namespace mzr {

struct Window::nthandle {
   MzrWindow   mzrWindow;
   GLFWwindow* glfwWindow;

   ~nthandle() {
      api_done_window(global_ctx, mzrWindow);
      glfwWindow = nullptr;
   }
};

Window::Window(int2 pos, int2 size, const char* title, std::initializer_list<int> hints_values) : pos(pos), size(size) {
   if (hints_values.size() > 0) {
      const int* start = hints_values.begin();
      for (size_t i = 0; i < hints_values.size(); ++i) {
         glfwWindowHint(start[i], 1);
      }
   }

   this->_underlying = new nthandle();
   MzrWindow window = api_create_window(global_ctx, title, pos.x, pos.y, size.x, size.y);
   GLFWwindow* underlying = getGlfw(window);
   _underlying->mzrWindow = window;
   _underlying->glfwWindow = underlying;

   glfwSetWindowUserPointer(underlying, this);
   glfwSetInputMode(underlying, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);

   this->_key_callback = noop<Event::KEY>;
   glfwSetKeyCallback(underlying, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      auto casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
      casted_window->_key_callback(casted_window, glfwKey_to_key(key), glfwEnum_to_action(action));
   });

   this->_ms_callback = noop<Event::MOUSE>;
   glfwSetMouseButtonCallback(underlying, [](GLFWwindow* window, int button, int action, int mods) {
      auto casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
      casted_window->_ms_callback(casted_window, glfwButton_to_msButton(button), glfwEnum_to_action(action));
   });

   this->_cursor_callback = noop<Event::CURSOR>;
   glfwSetCursorPosCallback(underlying, [](GLFWwindow* window, double xpos, double ypos) {
      auto casted_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
      casted_window->_cursor_callback(casted_window, xpos, ypos);
   });

   glfwMakeContextCurrent(underlying); // everything before this should not rely on the current context
   glViewport(pos.x, pos.y, size.x, size.y);
   glfwSwapInterval(0); // turn off VSync

   api_bind_shader(global_ctx, api_shader2d(global_ctx));
}

Window::Window(int2 pos, int2 size, const char* title) : Window(pos, size, title, {}) {}
Window::Window(int x, int y, int width, int height, const char* title, std::initializer_list<int> hints) : Window({x, y}, {width, height}, title, hints) {}
Window::Window(int x, int y, int width, int height, const char* title) : Window({x, y}, {width, height}, title, {}) {}

void Window::Close() {
   delete this->_underlying;
   this->_underlying = nullptr;
}

bool Window::ShouldClose() {
   return glfwWindowShouldClose(this->_underlying->glfwWindow);
}

void Window::SetShouldClose(bool value) {
   glfwSetWindowShouldClose(this->_underlying->glfwWindow, value);
}

void Window::SetVisible(bool value) {
   if (value)
      glfwShowWindow(this->_underlying->glfwWindow);
   else
      glfwHideWindow(this->_underlying->glfwWindow);
}

void Window::MakeCurrent() {
   glfwMakeContextCurrent(this->_underlying->glfwWindow);
}

void Window::SetTargetFPS(int fps) {
    if (fps < 1)
       return;
    else
       this->_frame_target = 1.0 / static_cast<double>(fps);
}

void Window::BeginFrame() {
   _frame_current = GetElapsedTime();
   _update = _frame_current - _frame_prev;
   _frame_prev = _frame_current;

   api_prepareFrame_window(global_ctx, this->_underlying->mzrWindow);
}

void Window::EndFrame() {
   for (std::size_t i = 0; i < _key_prev_states.size(); ++i) {
      _key_prev_states[i] = glfwGetKey(this->_underlying->glfwWindow, key_to_glfwKey(static_cast<Key>(i)));
   }
   for (std::size_t i = 0; i < _mouse_prev_states.size(); ++i) {
      _mouse_prev_states[i] = glfwGetMouseButton(this->_underlying->glfwWindow, i);
   }

   _frame_current = GetElapsedTime();
   _frame_time = _update + (_frame_current - _frame_prev);
   _frame_prev = _frame_current;

   if (_frame_time < _frame_target) {
      SleepTime(_frame_target - _frame_time);

      _frame_current = GetElapsedTime();
      double waitTime = _frame_current - _frame_prev;
      _frame_prev = _frame_current;

      _frame_time += waitTime;
   }
}

double Window::GetFrameTime() {
   return this->_frame_time;
}

template <>
void Window::SetCallback<Event::KEY>(Callback_t<Event::KEY> callback) {
   this->_key_callback = callback;
}
template <>
void Window::SetCallback<Event::MOUSE>(Callback_t<Event::MOUSE> callback) {
   this->_ms_callback = callback;
}
template <>
void Window::SetCallback<Event::CURSOR>(Callback_t<Event::CURSOR> callback) {
   this->_cursor_callback = callback;
}

float2 Window::GetMousePos() {
   double x, y;
   glfwGetCursorPos(this->_underlying->glfwWindow, &x, &y);
   return float2{static_cast<float>(x), static_cast<float>(y)};
}


// Input functions

bool Window::IsMouseDown(MouseButton button) {
   return GLFW_PRESS == glfwGetMouseButton(this->_underlying->glfwWindow, msButton_to_glfwButton(button));
}

bool Window::IsMousePressed(MouseButton button) {
   int glfw_button = msButton_to_glfwButton(button);
   int current_state = glfwGetMouseButton(this->_underlying->glfwWindow, glfw_button);

   return _mouse_prev_states[glfw_button] == GLFW_RELEASE && current_state == GLFW_PRESS;
}

bool Window::IsMouseReleased(MouseButton button) {
   int glfw_button = msButton_to_glfwButton(button);
   int current_state = glfwGetMouseButton(this->_underlying->glfwWindow, glfw_button);

   return _mouse_prev_states[glfw_button] == GLFW_PRESS && current_state == GLFW_RELEASE;
}

bool Window::IsKeyDown(Key key) {
   return GLFW_PRESS == glfwGetKey(this->_underlying->glfwWindow, key_to_glfwKey(key));
}

bool Window::IsKeyPressed(Key key) {
   int glfw_key = key_to_glfwKey(key);
   int current_state = glfwGetKey(this->_underlying->glfwWindow, glfw_key);

   return _key_prev_states[key] == GLFW_RELEASE && current_state == GLFW_PRESS;
}

bool Window::IsKeyReleased(Key key) {
   int glfw_key = key_to_glfwKey(key);
   int current_state = glfwGetKey(this->_underlying->glfwWindow, glfw_key);

   return _key_prev_states[key] == GLFW_PRESS && current_state == GLFW_RELEASE;
}


// Draw lifecycle

void Window::BeginDrawing() {
   glfwMakeContextCurrent(this->_underlying->glfwWindow);  
   float mat[16];
   createOrthographic(mat, 0, size.x, size.y, 0, -1, 1);
   api_set_projection(global_ctx, mat);
   this->_draw_last_marked = GetElapsedTime();
}

void Window::EndDrawing() {
   glfwSwapBuffers(this->_underlying->glfwWindow);
   this->_draw_time = GetElapsedTime() - this->_draw_last_marked;
}

double Window::GetDrawTime() {
   return this->_draw_time;
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
   /*
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
   draw();*/
}

void Window::BatchDraw(Rectangle* rectangles, std::size_t n, Color color) {
   /*std::size_t count = 0;
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
   draw();*/
}

void Window::BatchDraw(Circle* circles, std::size_t n, Color color) {}

// Static draw functions

// TODO:
// * another way to implement static drawables
// * this isn't elegant or performant enough
template<>
void Window::Draw<Triangle>(StaticDrawable<Triangle>& triangle) {
}
template<>
void Window::Draw<Rectangle>(StaticDrawable<Rectangle>& rectangle) {
}
template<>
void Window::Draw<Circle>(StaticDrawable<Circle>&) {}

// Regular draw functions
void Window::Draw() {}

/*
void Window::DrawText(Font font, const char* text, float size, float x, float y, Color color) {
   _current_shader->uploadUniform3f("textColor", color.r, color.g, color.b);
   if (font.isFaceLoaded(size) && !font.isFaceCurrent(size)) {
      font.makeFaceCurrent(size);
   }
   else {
      font.loadFace(size);
      font.makeFaceCurrent(size);
   }

   glActiveTexture(GL_TEXTURE0);

   while (*text != '\0') {
      float2 bearing = font.getBearing(*text);
      float2 size = font.getSize(*text);
      float xpos = x + bearing.x;
      float ypos = y + (size.y - bearing.y);

      float w = size.x;
      float h = size.y;

      // update VBO for each character
      float vertices[6*4] = {
         xpos,     ypos - h,   0.0f, 0.0f ,
         xpos,     ypos,       0.0f, 1.0f,
         xpos + w, ypos,       1.0f, 1.0f,

         xpos,     ypos - h,   0.0f, 0.0f,
         xpos + w, ypos,       1.0f, 1.0f,
         xpos + w, ypos - h,   1.0f, 0.0f
      };

      font.bindTexture(*text);
      //submit_immediate_call(GL_TRIANGLES, vertices, sizeof(vertices), 6);

      x += (font.getAdvance(*text) >> 6);
      ++text;
   }
}
*/

void Window::FlushRender() {
}

}

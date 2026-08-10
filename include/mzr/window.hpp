#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "input.hpp"
#include "shape.hpp"
#include "color.hpp"
#include <initializer_list>

namespace mzr {

class Window {
   void* _underlying; // GLFWwindow. I wanted to hide any internals from the user, and because of that, the Window class fell into an odd predicament. It needed a reference to a GLFWwindow, but it also needed to hide any GLFW-related things away from the user, so I decided to make this a void ptr. Of course, I could make Window an opaque type or follow the PImpl idiom, but those options didn't feel right to me. (Opaque type requires heap allocation, which isn't too bad since GLFWwindow does the same. For PImpl, I just dislike it.)
   Action _prev_state;

   double _last_marked;
   double _dt;

   void(*_callback)(Window* window, MouseButton button, Action action);

   public:
      int2 pos, size;

      Window(int2 pos, int2 size, const char* title, std::initializer_list<int> hints);
      Window(int2 pos, int2 size, const char* title);
      void Close();
      bool ShouldClose();

      void MakeCurrent();

      void MouseCallback( void(*callback)(Window* window, MouseButton button, Action action) );

      float2 GetMousePos();

      bool IsMouseDown(MouseButton button);
      bool IsMousePressed(MouseButton button);

      double GetFrameTime();

      void BeginDrawing();
      void Clear(Color color);

      void Draw(Triangle triangle, Color color);
      void Draw(Rectangle rectangle, Color color);
      void Draw(Circle circle, Color color);

      /*
      template <std::size_t N>
      void Draw(RegularDrawable<N>&& shape, Color color) {
         shape._set_color(color);
         shape._update_buffer(this->size);
         glDrawArrays(GL_TRIANGLES, 0, shape._count);
      }*/

      void EndDrawing();
};

}

#endif

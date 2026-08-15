#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "input.hpp"
#include "shape.hpp"
#include "color.hpp"
#include <initializer_list>
#include <unordered_map>
#include <vector>

namespace mzr {

enum class WindowHints {
   Visible,
   Resizable
};

class Window {
   std::unordered_map<std::size_t, std::size_t> _static_vbos;
   void* _underlying;

   Action _prev_state;

   double _last_marked;
   double _dt;

   void(*_callback)(Window* window, MouseButton button, Action action);

   public:
      int2 pos, size;

      Window(int2 pos, int2 size, const char* title, std::initializer_list<int> hints);
      Window(int2 pos, int2 size, const char* title);
      Window(int x, int y, int width, int height, const char* title, std::initializer_list<int> hints);
      Window(int x, int y, int width, int height, const char* title);

      void Close();
      bool ShouldClose();

      void SetVisible(bool value);

      void MakeCurrent();

      void MouseCallback( void(*callback)(Window* window, MouseButton button, Action action) );

      float2 GetMousePos();

      bool IsMouseDown(MouseButton button);
      bool IsMousePressed(MouseButton button);

      double GetFrameTime();

      void BeginDrawing();
      void Clear(Color color);

      void BatchDraw(Triangle* triangles, std::size_t n, Color color);
      void BatchDraw(Rectangle* rectangles, std::size_t n, Color color);
      void BatchDraw(Circle* circles, std::size_t n, Color color);

      template <typename T>
      void Draw(StaticDrawable<T> static_drawable);

      void Draw(Triangle triangle, Color color);
      void Draw(Rectangle rectangle, Color color);
      void Draw(Circle circle, Color color);

      void FlushRender();

      void EndDrawing();
};

}

#endif

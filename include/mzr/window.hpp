#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "core.hpp"
#include "event.hpp"
#include "input.hpp"
#include "shape.hpp"
#include "color.hpp"
#include <array>
#include <initializer_list>
#include <unordered_map>

namespace mzr {

enum class WindowHints {
   Visible,
   Resizable
};

class Window {
   std::unordered_map<std::size_t, std::size_t> _static_vbos;
   std::array<bool, Key::Count> _key_prev_states;
   std::array<bool, 3> _mouse_prev_states;

   void* _underlying;

   double _frame_target = 0;

   double _update = 0;
   double _frame_current = GetElapsedTime();
   double _frame_prev = _frame_prev;
   double _frame_time = 0;

   double _draw_last_marked = _frame_prev;
   double _draw_time = 0;

   Callback_t<Event::KEY>     _key_callback;
   Callback_t<Event::MOUSE>   _ms_callback;
   Callback_t<Event::CURSOR>  _cursor_callback;

   public:
      int2 pos, size;

      Window(int2 pos, int2 size, const char* title, std::initializer_list<int> hints);
      Window(int2 pos, int2 size, const char* title);
      Window(int x, int y, int width, int height, const char* title, std::initializer_list<int> hints);
      Window(int x, int y, int width, int height, const char* title);

      void Close();
      bool ShouldClose();
      void SetShouldClose(bool value);

      void SetVisible(bool value);

      void MakeCurrent();

      void SetTargetFPS(int fps);
      void BeginFrame();
      void EndFrame();
      double GetFrameTime();

      template <Event T>
      void SetCallback(Callback_t<T> callback);

      float2 GetMousePos();
      bool IsMouseDown(MouseButton button);
      bool IsMousePressed(MouseButton button);
      bool IsMouseReleased(MouseButton button);

      bool IsKeyDown(Key key);
      bool IsKeyPressed(Key key);
      bool IsKeyReleased(Key key);

      void BeginDrawing();
      void EndDrawing();
      double GetDrawTime();

      void Clear(Color color);

      void BatchDraw(Triangle* triangles, std::size_t n, Color color);
      void BatchDraw(Rectangle* rectangles, std::size_t n, Color color);
      void BatchDraw(Circle* circles, std::size_t n, Color color);

      template <typename T>
      void Draw(StaticDrawable<T>& static_drawable);

      void Draw(Triangle triangle, Color color);
      void Draw(Rectangle rectangle, Color color);
      void Draw(Circle circle, Color color);

      void FlushRender();
};

}

#endif

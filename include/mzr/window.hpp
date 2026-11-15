#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "core.hpp"
#include "event.hpp"
#include "input.hpp"
#include "shape.hpp"
#include "color.hpp"
#include <array>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

namespace mzr {

enum class WindowHints {
   Visible,
   Resizable
};

class Window {
   std::array<bool, Key::Count> _key_prev_states;
   std::array<bool, 3> _mouse_prev_states;

   struct nthandle;
   nthandle* _underlying;

   double _frame_target = 0;

   double _update = 0;
   double _frame_current = GetElapsedTime();
   double _frame_prev = _frame_prev; double _frame_time = 0;

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

      void Draw();

      template <typename T>
      void Draw(StaticDrawable<T>& static_drawable);

      template <typename Shape_T>
      requires std::is_base_of_v<Shape, Shape_T>
      void Draw(Shape_T& shape, Color color) {
         using TDrawable = IDrawable<Shape_T>;
         TDrawable::immediateDraw(shape, color);
      }

      template <typename Shape_T>
      requires std::is_base_of_v<Shape, Shape_T>
      void Draw(Shape_T&& shape, Color color) {
         using TDrawable = IDrawable<Shape_T>;
         TDrawable::immediateDraw(shape, color);
      }

      template <typename Shape_T>
      void Draw(Static<Shape_T>& staticShape) {
         using TDrawable = IDrawable<Shape_T>;
         staticShape.shape();
      }

      /*
      void Draw(Text text, Color color);

      template <typename ...Args>
      void Draw(Text fmt, Color color, const Args&&... args);

      void DrawText(Font font, const char* text, float size, float x, float y, Color color);

      void DrawText(const char* text, float size, float x, float y, Color color);

      template <typename ...Args>
      void DrawText(const char* fmt, float size, float x, float y, Color color, const Args&&... args) {
         Draw(std::format(fmt, args...).c_str(), size, x, y, color);
      }*/


      void FlushRender();
};

}

#endif

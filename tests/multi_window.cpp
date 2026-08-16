#include <mzr.hpp>

int main() {
   mzr::Init();

   mzr::Window window1 = { {0, 0}, {800, 800}, "first" }; // Windows can draw independently from one another. They can share resources either globally/application-wide (through first_context) or with one another.
   mzr::Window window2 = { {0, 0}, {800, 800}, "second" };

   mzr::Triangle triangleWin1 = { {0, 0}, {400, 0}, {200, 200}};
   mzr::Rectangle rectangleWin2 = {200, 200, 400, 400};

   while (!window1.ShouldClose() && !window2.ShouldClose()) {
      mzr::ProcessEvents(); // You should only call this once

      window1.BeginFrame();
         window1.BeginDrawing();
         window1.Clear(mzr::Color::WHITE);
         window1.Draw(triangleWin1, mzr::Color::RED);
         window1.EndDrawing();
      window1.EndFrame();

      window2.BeginFrame();
         window2.BeginDrawing();
         window2.Clear(mzr::Color::WHITE);
         window2.Draw(rectangleWin2, mzr::Color::RED);
         window2.EndDrawing();
      window2.EndFrame();
   }

   window1.Close();
   window2.Close();

   mzr::Close();
   return 0;
}

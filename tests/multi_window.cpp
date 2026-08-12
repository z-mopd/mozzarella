#include <mzr.hpp>

int main() {
   mzr::libinit();

   mzr::Window window1 = { {0, 0}, {800, 800}, "first" };
   mzr::Window window2 = { {0, 0}, {800, 800}, "second" };

   mzr::Triangle triangleWin1 = { {0, 0}, {400, 0}, {200, 200}};
   mzr::Rectangle rectangleWin2 = {200, 200, 400, 400};

   while (!window1.ShouldClose() && !window2.ShouldClose()) {
      window1.BeginDrawing();
      window1.Clear(mzr::Color::WHITE);
      window1.Draw(triangleWin1, mzr::Color::RED);
      window1.EndDrawing();

      window2.BeginDrawing();
      window2.Clear(mzr::Color::WHITE);
      window2.Draw(rectangleWin2, mzr::Color::RED);
      window2.EndDrawing();

      mzr::ProcessEvents(); // You should only call this once
   }

   window1.Close();
   window2.Close();

   mzr::libclose();
   return 0;
}

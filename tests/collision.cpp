#include <iostream>
#include <mzr.hpp>

int main() {
   mzr::Init();

   mzr::Triangle  triangle    = {{300, 0}, {0, 300}, {300, 300}};
   mzr::Rectangle rectangle   = {{200, 200}, {400, 400}};
   mzr::Circle    circle      = {{500, 500}, 40};

   mzr::Window window{{0, 0}, {800, 800}, "collision test"};

   window.SetTargetFPS(60);

   if (mzr::CheckCollision(rectangle, triangle)) {
      std::cout << "rectangle collides with triangle\n";
   }

   if (mzr::CheckCollision(triangle, rectangle)) { // equivalent to mzr::CheckCollision(rectangle, triangle)
      std::cout << "triangle collides with rectangle\n";
   }

   if (mzr::CheckCollision(circle, rectangle)) {
      std::cout << "circle collides with rectangle\n";
   }

   while (!window.ShouldClose()) {
      mzr::ProcessEvents();
      window.BeginFrame();

      window.BeginDrawing();
         window.Clear(mzr::Color::BLACK);

         window.Draw(triangle, mzr::Color::RED);
         window.Draw(rectangle, mzr::Color::BLUE);
         window.Draw(circle, mzr::Color::WHITE);
      window.EndDrawing();

      window.EndFrame();
   }

   window.Close();
   mzr::Close();
}

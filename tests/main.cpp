#include <cstdio>
#include <mzr/color.hpp>
#include <mzr/input.hpp>
#include <mzr/shape.hpp>
#include <mzr/window.hpp>
#include <mzr/events.hpp>
#include <mzr/init.hpp>

#include <cmath>
#include <iostream>

int main() {
   mzr::libinit();

   int winWidth = 800;
   int winHeight = 800;

   mzr::Window myWindow = {{0, 0}, {winWidth, winHeight}, "this is a window"};

   mzr::Triangle myTriangle = {{0, 0}, {400, 0}, {200, 200}};
   mzr::Rectangle myRec = {{200, 300}, {400, 200}};

   myWindow.MouseCallback([](mzr::Window* window, MouseButton button, Action action) {
      if (button == MouseButton::LEFT && action == Action::PRESS) {
         std::cout << "left click\n";
      }
   });

   while (!myWindow.ShouldClose()) {
      std::cout << myWindow.GetFrameTime() << '\n';
      if (myTriangle.CheckCollisionPoint(myWindow.GetMousePos()) && myWindow.IsMousePressed(MouseButton::LEFT)) {
         std::cout << "triangle clicked\n";
      }

      if (myRec.CheckCollisionPoint(myWindow.GetMousePos()) && myWindow.IsMousePressed(MouseButton::LEFT)) {
         std::cout << "clicked!\n";
      }

      myWindow.BeginDrawing();
      myWindow.Clear(mzr::Color::BLACK);

      myWindow.Draw(myRec, mzr::Color::RED);
      myWindow.Draw(myTriangle, mzr::Color::RED);

      mzr::ProcessEvents();
      myWindow.EndDrawing();
   }

   myWindow.Close();

   mzr::libclose();

   return 0;
}

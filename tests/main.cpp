#include <cstdio>
#include <mzr/color.hpp>
#include <mzr/input.hpp>
#include <mzr/shape.hpp>
#include <mzr/window.hpp>
#include <mzr/events.hpp>
#include <mzr/init.hpp>

#include <iostream>

int main() {
   mzr::Init();

   int winWidth = 800;
   int winHeight = 800;

   mzr::Window myWindow = {{0, 0}, {winWidth, winHeight}, "this is a window"};

   mzr::Triangle myTriangle = {{0, 0}, {400, 0}, {200, 200}};
   mzr::Rectangle myRec = {{200, 300}, {400, 200}};

   myWindow.MouseCallback([](mzr::Window* window, mzr::MouseButton button, mzr::Action action) {
      if (button == mzr::MouseButton::LEFT && action == mzr::Action::PRESS) {
         std::cout << "left click\n";
      }
   });

   while (!myWindow.ShouldClose()) {
      printf("%f\n", myWindow.GetFrameTime());
      if (myTriangle.CheckCollisionPoint(myWindow.GetMousePos()) && myWindow.IsMousePressed(mzr::MouseButton::LEFT)) {
         std::cout << "triangle clicked\n";
      }

      if (myRec.CheckCollisionPoint(myWindow.GetMousePos()) && myWindow.IsMousePressed(mzr::MouseButton::LEFT)) {
         std::cout << "clicked!\n";
      }

      myWindow.BeginDrawing();
      myWindow.Clear(mzr::Color::BLACK);

      myWindow.Draw(myTriangle, mzr::Color::RED);
      myWindow.Draw(myRec, mzr::Color::RED);

      mzr::ProcessEvents();
      myWindow.EndDrawing();
   }

   myWindow.Close();

   mzr::Close();

   return 0;
}

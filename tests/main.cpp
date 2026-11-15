#include <mzr/color.hpp>
#include <mzr/input.hpp>
#include <mzr/shape.hpp>
#include <mzr/window.hpp>
#include <mzr/event.hpp>
#include <mzr/core.hpp>

#include <iostream>

int main() {
   mzr::Init();

   int winWidth = 800;
   int winHeight = 800;

   mzr::Window myWindow = {{0, 0}, {winWidth, winHeight}, "this is a window"};

   mzr::Triangle myTriangle = {{0, 0}, {400, 0}, {200, 200}};
   mzr::Rectangle myRec = {{200, 300}, {400, 200}};
   mzr::Circle myCircle = {{400, 600}, 150};

   myWindow.SetTargetFPS(60);

   myWindow.SetCallback<mzr::Event::MOUSE>([](mzr::Window* window, mzr::MouseButton button, mzr::Action action) {
      if (button == mzr::MouseButton::LEFT && action == mzr::Action::PRESS) {
         std::cout << "left click\n";
      }
   });

   while (!myWindow.ShouldClose()) {
      myWindow.BeginFrame();
         if (mzr::CheckCollisionPoint(myTriangle, myWindow.GetMousePos()) && myWindow.IsMousePressed(mzr::MouseButton::LEFT)) {
            std::cout << "triangle clicked\n";
         }

         if (mzr::CheckCollisionPoint(myRec, myWindow.GetMousePos()) && myWindow.IsMousePressed(mzr::MouseButton::LEFT)) {
            std::cout << "rec clicked!\n";
         }

         myWindow.BeginDrawing();
            myWindow.Clear(mzr::Color::BLACK);

            myWindow.Draw(myTriangle, mzr::Color::RED);
            myWindow.Draw(myRec, mzr::Color::RED);
            myWindow.Draw(myCircle, mzr::Color::WHITE);
         myWindow.EndDrawing();
      myWindow.EndFrame();
      mzr::ProcessEvents(); // You should mzr::ProcessEvents after or before any frames
   }

   myWindow.Close();
   mzr::Close(); // After calling mzr::Close(), you can call mzr::Init() to restart the lifecycle

   return 0;
}

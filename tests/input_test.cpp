#include <mzr.hpp>

#include <iostream>

int main() {
   mzr::Init();

   mzr::Window inputWindow = {0, 0, 800, 800, "input"};

   inputWindow.SetCallback<mzr::Event::CURSOR>([](mzr::Window* window, double x, double y) {
      std::cout << "x: " << x << '\n' 
                << "y: " << y << '\n';
   });

   inputWindow.SetCallback<mzr::Event::KEY>([](mzr::Window* window, mzr::Key key, mzr::Action action) {
      if (key == mzr::Key::D && action == mzr::Action::PRESS) {
         window->SetShouldClose(true); // Avoid calling window->Close() inside this function because doing so would cause undefined behavior. This is because the while loop up ahead would exit and another Close() function would be called on inputWindow.
      }

      if (key == mzr::Key::C && action == mzr::Action::REPEAT) {
         std::cout << "the C key was held down!\n";
      }
   });

   while (!inputWindow.ShouldClose()) {
      mzr::ProcessEvents();
      inputWindow.BeginFrame();
         if (inputWindow.IsMouseDown(mzr::MouseButton::LEFT)) {
            std::cout << "the left mouse button is currently down!\n";
         }

         if (inputWindow.IsMousePressed(mzr::MouseButton::RIGHT)) {
            std::cout << "the right mouse button was down for this frame!\n";
         }

         if (inputWindow.IsMouseReleased(mzr::MouseButton::RIGHT)) {
            std::cout << "the right mouse button was released for this frame!\n";
         }

         if (inputWindow.IsKeyPressed(mzr::Key::LSHIFT)) {
            std::cout << "left shift was down for this frame!\n";
         }

         if (inputWindow.IsKeyReleased(mzr::Key::LSHIFT)) {
            std::cout << "left shift was released for this frame!\n";
         }
      inputWindow.EndFrame();
   }

   inputWindow.Close();
   mzr::Close();

   return 0;
}

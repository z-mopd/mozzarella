#include <cstdio>
#include <mzr.hpp>

int main() {
   mzr::Init();

   mzr::Window myWindow = { {0, 0}, {800, 800}, "hey" };

   mzr::StaticDrawable<mzr::Rectangle> myStaticRec = {mzr::Color::RED, 0, 0, 400, 200};

   while (!myWindow.ShouldClose()) {
      mzr::ProcessEvents();
      printf("%f\n", myWindow.GetFrameTime()); // Usually, FrameTime is longer than DrawTime.
                                                       // FrameTime is: time spent between BeginFrame() and DrawFrame()
      printf("%f\n", myWindow.GetDrawTime());  // DrawTime is: time spent executing draw functions

      myWindow.BeginFrame();

         myWindow.BeginDrawing();

         myWindow.Clear(mzr::Color::BLACK);

         myWindow.Draw(myStaticRec);

         myWindow.EndDrawing();
      myWindow.EndFrame();
   }

   myWindow.Close();

   mzr::Close();
}

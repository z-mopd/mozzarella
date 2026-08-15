#include <cstdio>
#include <mzr.hpp>

int main() {
   mzr::Init();

   mzr::Window myWindow = { {0, 0}, {800, 800}, "hey" };

   mzr::StaticDrawable<mzr::Rectangle> myStaticRec = {mzr::Color::RED, 0, 0, 400, 200};

   while (!myWindow.ShouldClose()) {
      printf("%f\n", myWindow.GetFrameTime());

      mzr::ProcessEvents();
      myWindow.BeginDrawing();

      myWindow.Clear(mzr::Color::BLACK);

      myWindow.Draw(myStaticRec);

      myWindow.EndDrawing();
   }

   myWindow.Close();

   mzr::Close();
}

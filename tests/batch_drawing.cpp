#include <array>
#include <cstdio>
#include <mzr.hpp>

int main() {
   mzr::Init();

   mzr::Window batchedWindow = { {0, 0}, {800, 800}, "batchedWindow"};

   std::array<mzr::Triangle, (8*8)> triangles;

   std::size_t index = 0;

   for (std::size_t y = 0; y < 8; ++y) {
      for (std::size_t x = 0; x < 8; ++x) {
         float xf = static_cast<float>(x*100);
         float yf = static_cast<float>(y*100);

         triangles[index] = mzr::Triangle(
            {xf, yf+100},
            {xf+50, yf},
            {xf+100, yf+100}
         );

         ++index;
      }
   }

   while (!batchedWindow.ShouldClose()) {
      mzr::ProcessEvents();
      batchedWindow.BeginFrame();
      printf("%f\n", batchedWindow.GetFrameTime());
      batchedWindow.BeginDrawing();

      batchedWindow.Clear(mzr::Color::WHITE);

      batchedWindow.BatchDraw(triangles.data(), triangles.size(), mzr::Color::RED);

      batchedWindow.EndDrawing();
      batchedWindow.EndFrame();
   }

   batchedWindow.Close();

   mzr::Close();
}

#include "_common.hpp"
#include "_render.hpp"
#include <thread>

#include "core.hpp"

double init_time;

namespace mzr {

bool Init() {
   return r_init(&init_time);
}

double GetElapsedTime() {
   return glfwGetTime();
}

double GetCurrentTime() {
   return glfwGetTime() - init_time;
}

void SleepTime(double seconds) {
   if (seconds < 0)
      return;

   long nsec = seconds*1000000000L;
   std::this_thread::sleep_for(std::chrono::nanoseconds(nsec));
}

void Close() {
   r_close();
}

}

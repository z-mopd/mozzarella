#include "_common.hpp"
#include "event.hpp"

#include "platform/api.h"

#include <thread>

#include "core.hpp"

MzrApiCtx global_ctx = nullptr;
double init_time;

namespace mzr {

bool IsInitialized() {
   return global_ctx != nullptr;
}

bool IsClosed() {
   return !IsInitialized();
}

bool Init() {
   if (IsInitialized()) return true;

   global_ctx = api_ctx();
   return api_init(global_ctx, &init_time);
}

void Close() {
   if (IsClosed()) return;

   api_done(global_ctx);
   global_ctx = nullptr;
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

void ProcessEvents() {
   glfwPollEvents();
}

}

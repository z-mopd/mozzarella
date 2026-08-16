#include "_common.hpp"
#include "_render.hpp"

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

void Close() {
   r_close();
}

}

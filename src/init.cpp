#include "init.hpp"
#include "_render.hpp"

bool mzr::Init() {
   return r_init();
}

void mzr::Close() {
   r_close();
}

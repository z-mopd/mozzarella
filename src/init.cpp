#include "init.hpp"
#include "_render.hpp"

bool mzr::libinit() {
   return r_init();
}

void mzr::libclose() {
   r_close();
}

#ifndef _HELPER_HPP
#define _HELPER_HPP

#include "_common.hpp"
#include "data_types.hpp"
#include "input.hpp"

GLfloat2 f2_to_glf2(mzr::float2 f2) {
   return GLfloat2{f2.x, f2.y};
}

mzr::Action glfwEnum_to_action(int value) {
   switch(value) {
      case GLFW_RELEASE:
         return mzr::Action::RELEASE;

      case GLFW_PRESS:
         return mzr::Action::PRESS;

      case GLFW_REPEAT:
         return mzr::Action::REPEAT;

      default:
         return mzr::Action::UNKNOWN;
   }
}

#endif

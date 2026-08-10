#ifndef _HELPER_HPP
#define _HELPER_HPP

#include "_common.hpp"
#include "data_types.hpp"
#include "input.hpp"

GLfloat2 f2_to_glf2(mzr::float2 f2) {
   return GLfloat2{f2.x, f2.y};
}

Action glfwEnum_to_action(int value) {
   switch(value) {
      case GLFW_RELEASE:
         return Action::RELEASE;

      case GLFW_PRESS:
         return Action::PRESS;

      case GLFW_REPEAT:
         return Action::REPEAT;

      default:
         return Action::UNKNOWN;
   }
}

#endif

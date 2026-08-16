#ifndef _HELPER_HPP
#define _HELPER_HPP

#include "_common.hpp"
#include "input.hpp"

int key_to_glfwKey(mzr::Key key) {
   switch (key) {
      case mzr::Key::LSHIFT:
         return GLFW_KEY_LEFT_SHIFT;
   }

   static const int first = static_cast<int>(mzr::Key::A);
   static const int last = static_cast<int>(mzr::Key::Z);

   const int k = static_cast<int>(key);

   if (k >= first && k <= last)
      return GLFW_KEY_A + (k - first);

   return GLFW_DONT_CARE;
}

mzr::Key glfwKey_to_key(int key) {
   switch (key) {
      case GLFW_KEY_LEFT_SHIFT:
         return mzr::Key::LSHIFT;
   }

   return static_cast<mzr::Key>(key - GLFW_KEY_A);
}

int msButton_to_glfwButton(mzr::MouseButton button) {
   switch (button) {
      case mzr::LEFT:
         return GLFW_MOUSE_BUTTON_LEFT;

      case mzr::RIGHT:
         return GLFW_MOUSE_BUTTON_RIGHT;

      case mzr::MIDDLE:
         return GLFW_MOUSE_BUTTON_MIDDLE;

      default:
         return GLFW_DONT_CARE;
   }
}

mzr::MouseButton glfwButton_to_msButton(int button) {
   switch (button) {
      case GLFW_MOUSE_BUTTON_LEFT:
         return mzr::MouseButton::LEFT;

      case GLFW_MOUSE_BUTTON_RIGHT:
         return mzr::MouseButton::RIGHT;

      case GLFW_MOUSE_BUTTON_MIDDLE:
         return mzr::MouseButton::MIDDLE;

      default:
         return mzr::MouseButton::ERROR;
   }
}

mzr::Action glfwEnum_to_action(int value) {
   switch (value) {
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

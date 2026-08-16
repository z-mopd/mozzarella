#ifndef _HELPER_HPP
#define _HELPER_HPP

#include "_common.hpp"
#include "input.hpp"
#include <iostream>

static const int first_let = static_cast<int>(mzr::Key::A);
static const int last_let = static_cast<int>(mzr::Key::Z);

static const int first_num = static_cast<int>(mzr::Key::ZERO);
static const int last_num = static_cast<int>(mzr::Key::NINE);

int key_to_glfwKey(mzr::Key key) {
   switch (key) {
      case mzr::Key::LSHIFT:
         return GLFW_KEY_LEFT_SHIFT;
      case mzr::Key::RSHIFT:
         return GLFW_KEY_RIGHT_SHIFT;

      case mzr::Key::ESCAPE:
         return GLFW_KEY_ESCAPE;
      case mzr::Key::ENTER:
         return GLFW_KEY_ENTER;
      case mzr::Key::TAB:
         return GLFW_KEY_TAB;
      case mzr::Key::BACKSPACE:
         return GLFW_KEY_BACKSPACE;
   }

   const int k = static_cast<int>(key);

   if (k >= first_let && k <= last_let)
      return GLFW_KEY_A + (k - first_let);

   if (k >= first_num && k <= last_num) {
      return GLFW_KEY_0 + (k - first_num);
   }

   return GLFW_DONT_CARE;
}

mzr::Key glfwKey_to_key(int key) {
   switch (key) {
      case GLFW_KEY_LEFT_SHIFT:
         return mzr::Key::LSHIFT;
      case GLFW_KEY_RIGHT_SHIFT:
         return mzr::Key::RSHIFT;

      case GLFW_KEY_ESCAPE:
         return mzr::Key::ESCAPE;
      case GLFW_KEY_ENTER:
         return mzr::Key::ENTER;
      case GLFW_KEY_TAB:
         return mzr::Key::TAB;
      case GLFW_KEY_BACKSPACE:
         return mzr::Key::BACKSPACE;
   }

   if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
      return static_cast<mzr::Key>(first_let + (key - GLFW_KEY_A));

   if (key >= GLFW_KEY_0 && key <= GLFW_KEY_0)
      return static_cast<mzr::Key>(first_num + (key - GLFW_KEY_0));

   return mzr::Key::Count;
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

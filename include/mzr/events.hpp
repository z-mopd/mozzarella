#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "input.hpp"

namespace mzr {

class Window;

enum class Event {
   KEY,
   MOUSE,
   CURSOR
};

template <Event>
struct Callback;

template <>
struct Callback<Event::KEY> {
   using type = void(*)(Window* window, Key key, Action action);
   static void noop(Window*, Key, Action) {}
};

template <>
struct Callback<Event::MOUSE> {
   using type = void(*)(Window* window, MouseButton button, Action action);
   static void noop(Window*, MouseButton, Action) {}
};

template <>
struct Callback<Event::CURSOR> {
   using type = void(*)(Window* window, double x, double y);
   static void noop(Window*, double, double) {}
};

template <Event T>
using Callback_t = typename Callback<T>::type;

template <Event T>
constexpr Callback_t<T> noop = &Callback<T>::noop;

// TODO: add window-level event processing rather than global event processing

void ProcessEvents();

}

#endif

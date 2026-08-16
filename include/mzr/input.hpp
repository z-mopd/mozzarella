#ifndef INPUT_HPP
#define INPUT_HPP

namespace mzr {

enum Key {
   A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

   LSHIFT,
   RSHIFT,
   Count
};

enum MouseButton {
   LEFT,
   RIGHT,
   MIDDLE,
   ERROR
};

enum Action {
   RELEASE,
   PRESS,
   REPEAT,
   UNKNOWN
};

}

#endif

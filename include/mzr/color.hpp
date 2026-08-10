#ifndef COLOR_HPP
#define COLOR_HPP

#include "../mzr.hpp"
#include <cstdint>

namespace mzr {

class Color {
   public:
      float r, g, b, a;
      Color(float r, float g, float b);
      Color(float r, float g, float b, float a);

      Color normalized() const;

      static const Color RED;
      static const Color BLUE;
      static const Color BLACK;
      static const Color WHITE;
};

}

#endif

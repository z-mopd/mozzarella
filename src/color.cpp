#include "color.hpp"

namespace mzr {

Color::Color(float r, float g, float b) : r(r), g(g), b(b), a(255) {}
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

Color Color::normalized() const {
   return Color(this->r / 255, this->g / 255, this->b / 255, this->a / 255);
}

const Color Color::RED = {255, 0, 0};
const Color Color::BLUE = {0, 0, 255};
const Color Color::BLACK = {0, 0, 0};
const Color Color::WHITE = {255, 255, 255};

}

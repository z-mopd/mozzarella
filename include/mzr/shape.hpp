#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "data_types.hpp"
#include <cstdint>

#define CIRCLE_DEFAULT_SEGMENTS 100

namespace mzr {

class Shape {
   protected:
      std::uint32_t _vbo;
      std::uint32_t _vao;

   public:
      virtual bool CheckCollisionPoint(float2 point) = 0;

      std::uint32_t get_bid() const;
      std::uint32_t get_sid() const;
};

/*
template <std::size_t N>
class RegularDrawable {
   protected:
      std::array<float, N + 4> _vertices;
      std::size_t _count;
      unsigned int _vbo;
      unsigned int _vao;

      virtual void _update_buffer(int2 win_size) = 0;
      virtual void _set_color(Color color) = 0;

   friend class Window;
};*/

template <typename Shape_T>
class StaticDrawable {};

class Triangle : public Shape {
   public:
      float2 p1, p2, p3;
      Triangle(float2 p1, float2 p2, float2 p3);

      bool CheckCollisionPoint(float2 point) override;
};

class Rectangle : public Shape {
   public:
      float2 pos, size;

      Rectangle(float2 pos, float2 size);

      bool CheckCollisionPoint(float2 point) override;
};

class Circle : public Shape {
   public:
      float2 pos;
      float radius;

      Circle(float2 pos, float radius);

      bool CheckCollisionPoint(float2 point) override;
};

}

#endif

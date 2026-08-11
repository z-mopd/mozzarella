#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "color.hpp"
#include "data_types.hpp"
#include <cstdint>
#include <utility>

#define CIRCLE_DEFAULT_SEGMENTS 100

namespace mzr {

class Shape {
   public:
      virtual bool CheckCollisionPoint(float2 point) = 0;
};

template <typename Shape_T>
class StaticDrawable {
   Shape_T _shape;
   Color color;

   public:
      template <typename... Args>
      StaticDrawable(Color color, Args&&... args) : color(color), _shape(std::forward<Args>(args)...) {}
};

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
      Rectangle(float x, float y, float width, float height);

      bool CheckCollisionPoint(float2 point) override;
};

class Circle : public Shape {
   public:
      const std::size_t SEGMENTS;
      float2 pos;
      float radius;

      Circle(float2 pos, float radius);
      Circle(float2 pos, float radius, std::size_t segments);

      bool CheckCollisionPoint(float2 point) override;
};

}

#endif

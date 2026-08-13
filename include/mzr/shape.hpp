#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "color.hpp"
#include "data_types.hpp"
#include <utility>

#define CIRCLE_DEFAULT_SEGMENTS 100

namespace mzr {

class Shape {
   public:
      virtual bool CheckCollisionPoint(float2 point) const = 0;
};

// TODO: implement static drawing
template <typename Shape_T>
class StaticDrawable {
   static std::size_t NEXT_ID;

   Shape_T _shape;
   Color _color;

   const std::size_t _id;

   public:
      template <typename... Args>
      StaticDrawable(Color color, Args&&... args) : _id(++NEXT_ID), _color(color), _shape(std::forward<Args>(args)...) {}

   friend class Window;
};
template <typename Shape_T>
std::size_t StaticDrawable<Shape_T>::NEXT_ID = 0;

class Triangle : public Shape {
   public:
      float2 p1, p2, p3;
      Triangle(float2 p1, float2 p2, float2 p3);

      bool CheckCollisionPoint(float2 point) const override;
};

class Rectangle : public Shape {
   public:
      float2 pos, size;

      Rectangle(float2 pos, float2 size);
      Rectangle(float x, float y, float width, float height);

      bool CheckCollisionPoint(float2 point) const override;
};

class Circle : public Shape {
   public:
      const std::size_t SEGMENTS;
      float2 pos;
      float radius;

      Circle(float2 pos, float radius);
      Circle(float2 pos, float radius, std::size_t segments);

      bool CheckCollisionPoint(float2 point) const override;
};

}

#endif

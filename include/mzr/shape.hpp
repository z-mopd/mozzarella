#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "color.hpp"
#include "data_types.hpp"
#include <utility>

#define CIRCLE_DEFAULT_SEGMENTS 100

namespace mzr {

class Shape {
   protected:
      virtual std::size_t _count() const = 0;
      virtual void GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const = 0;

   public:
      virtual bool CheckCollisionPoint(float2 point) const = 0;

   friend class Window;
};

template <typename Shape_T>
class StaticDrawable {
   static std::size_t _NEXT_ID;

   Shape_T _shape;
   Color _color;

   const std::size_t _id;

   public:
      template <typename... Args>
      StaticDrawable(Color color, Args&&... args) : _id(++_NEXT_ID), _color(color), _shape(std::forward<Args>(args)...) {}

   friend class Window;
};
template <typename Shape_T>
std::size_t StaticDrawable<Shape_T>::_NEXT_ID = 0;

class Triangle : public Shape {
   protected:
      std::size_t _count() const override;
      void GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const override;

   public:
      float2 p1, p2, p3;

      Triangle();
      Triangle(float2 p1, float2 p2, float2 p3);

      bool CheckCollisionPoint(float2 point) const override;

   friend class Window;
};

class Rectangle : public Shape {
   protected:
      std::size_t _count() const override;
      void GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const override;

   public:
      float2 pos, size;

      Rectangle();
      Rectangle(float2 pos, float2 size);
      Rectangle(float x, float y, float width, float height);

      bool CheckCollisionPoint(float2 point) const override;

   friend class Window;
};

class Circle : public Shape {
   protected:
      std::size_t _count() const override;
      void GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const override;

   public:
      const std::size_t SEGMENTS;
      float2 pos;
      float radius;

      Circle();
      Circle(float2 pos, float radius);
      Circle(float2 pos, float radius, std::size_t segments);

      bool CheckCollisionPoint(float2 point) const override;

   friend class Window;
};

}

#endif

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "color.hpp"
#include "data_types.hpp"
#include <utility>

namespace mzr {

class Shape {};

template <typename Shape_T>
struct IDrawable {
   static constexpr std::size_t vertexCount() {return Shape_T::vertexCount();}

   static void immediateDraw(const Shape_T& obj, Color color) {
      Shape_T::immediateDraw(obj, color);
   }
};

template <typename Shape_T>
struct IShape {
   static bool CheckCollisionPoint(const Shape_T& obj, float2 point) { return Shape_T::CheckCollisionPoint(obj, point); }

   template <typename Shape_U>
   static bool CheckCollision(const Shape_T& obj, const Shape_U& other) { return Shape_T::CheckCollision(obj, other); }

   template <typename Shape_U>
   struct collision_method_with {
      static constexpr bool value = requires(const Shape_T& obj, const Shape_U& other) {
         { Shape_T::CheckCollision(obj, other) } -> std::same_as<bool>;
      };
   };
};

template <typename Shape_T>
bool CheckCollisionPoint(const Shape_T& shape, float2 point) {
   return IShape<Shape_T>::CheckCollisionPoint(shape, point);
}

template <typename Shape_T, typename Shape_U>
   requires IShape<Shape_T>::template collision_method_with<Shape_U>::value || IShape<Shape_U>::template collision_method_with<Shape_T>::value
bool CheckCollision(const Shape_T& shape, const Shape_U& other) {
   if constexpr (IShape<Shape_T>::template collision_method_with<Shape_U>::value) {
      return IShape<Shape_T>::CheckCollision(shape, other);
   }
   else {
      return IShape<Shape_U>::CheckCollision(other, shape);
   }
}

// replacement for StaticDrawable
template <typename Shape_T>
class Static {
   using TDrawable = IDrawable<Shape_T>;

   Shape_T _shape;
   Color   _color;

   public:
      template <typename... Args>
      Static(Color color, Args&&... args) : _color(color), _shape(std::forward<Args>(args)...) {}

      const Shape_T& shape() {return _shape;}
};

template <typename Shape_T>
class StaticDrawable {
   static std::size_t _NEXT_ID;

   Shape_T _shape;
   Color _color;

   const std::size_t _id;

   public:
      template <typename... Args>
      StaticDrawable(Color color, Args&&... args) : _id(_NEXT_ID++), _color(color), _shape(std::forward<Args>(args)...) {}

   friend class Window;
};
template <typename Shape_T>
std::size_t StaticDrawable<Shape_T>::_NEXT_ID = 0;

class Triangle : Shape {
   friend class IShape<Triangle>;
   static bool CheckCollisionPoint(const Triangle& obj, float2 point);
   static bool CheckCollision(const Triangle& obj, const Triangle& other);

   friend class IDrawable<Triangle>;
   static constexpr std::size_t vertexCount() { return 3; }
   static void immediateDraw(const Triangle& obj, Color color);

   public:
      float2 p1, p2, p3;

      Triangle();
      Triangle(float2 p1, float2 p2, float2 p3);
};

class Rectangle : Shape {
   friend class IShape<Rectangle>;
   static bool CheckCollisionPoint(const Rectangle& obj, float2 point);
   static bool CheckCollision(const Rectangle& obj, const Rectangle& other);
   static bool CheckCollision(const Rectangle& obj, const Triangle& other);

   friend class IDrawable<Rectangle>;
   static constexpr std::size_t vertexCount() { return 6; }
   static void immediateDraw(const Rectangle& obj, Color color);

   public:
      float2 pos, size;

      Rectangle();
      Rectangle(float2 pos, float2 size);
      Rectangle(float x, float y, float width, float height);
};

class Circle : Shape {
   friend class IShape<Circle>;
   static bool CheckCollisionPoint(const Circle& obj, float2 point);
   static bool CheckCollision(const Circle& obj, const Circle& other);
   static bool CheckCollision(const Circle& obj, const Triangle& other);
   static bool CheckCollision(const Circle& obj, const Rectangle& other);

   friend class IDrawable<Circle>;
   static constexpr std::size_t _segments = 64;
   static constexpr std::size_t vertexCount() { return _segments; }
   static void immediateDraw(const Circle& obj, Color color);

   public:
      float2 pos;
      float radius;

      Circle();
      Circle(float2 pos, float radius);
};

}

#endif

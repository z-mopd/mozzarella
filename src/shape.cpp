#include "shape.hpp"
#include "_common.hpp"

#include "color.hpp"
#include "platform/api.h"
#include <cmath>

namespace mzr {

// Triangle
Triangle::Triangle() : p1({0, 0}), p2({0, 0}), p3({0, 0}) {}
Triangle::Triangle(float2 p1, float2 p2, float2 p3) : p1(p1), p2(p2), p3(p3) {}

bool Triangle::CheckCollisionPoint(const Triangle& obj, float2 point) {
    double areaOrig = fabs((obj.p2.x - obj.p1.x) * (obj.p3.y - obj.p1.y) - (obj.p3.x - obj.p1.x) * (obj.p2.y - obj.p1.y));
    double area1 = fabs((obj.p1.x - point.x) * (obj.p2.y - point.y) - (obj.p2.x - point.x) * (obj.p1.y - point.y));
    double area2 = fabs((obj.p2.x - point.x) * (obj.p3.y - point.y) - (obj.p3.x - point.x) * (obj.p2.y - point.y));
    double area3 = fabs((obj.p3.x - point.x) * (obj.p1.y - point.y) - (obj.p1.x - point.x) * (obj.p3.y - point.y));
    return fabs(area1 + area2 + area3 - areaOrig) < 1e-6;
}

bool Triangle::CheckCollision(const Triangle& obj, const Triangle& other) {
   return true;
}

void Triangle::immediateDraw(const Triangle& obj, Color color) {
   Color normal = color.normalized();
   api_set_color(global_ctx, normal.r, normal.g, normal.b, normal.a);
   api_draw_triangle(global_ctx,
      obj.p1.x, obj.p1.y,
      obj.p2.x, obj.p2.y,
      obj.p3.x, obj.p3.y
   );
}

// Rectangle
Rectangle::Rectangle() : pos({0, 0}), size({0, 0}) {}
Rectangle::Rectangle(float2 pos, float2 size) : pos(pos), size(size) {}
Rectangle::Rectangle(float x, float y, float width, float height) : pos{x, y}, size{width, height} {}

bool Rectangle::CheckCollisionPoint(const Rectangle& obj, float2 point) {
   float min_x = obj.pos.x;
   float min_y = obj.pos.y;

   float max_x = obj.pos.x + obj.size.x;
   float max_y = obj.pos.y + obj.size.y;

   return (min_x <= point.x && point.x <= max_x && min_y <= point.y && point.y <= max_y);
}

bool Rectangle::CheckCollision(const Rectangle& obj, const Rectangle& other) {
   return (
      obj.pos.x < other.pos.x + other.size.x &&
      obj.pos.x + obj.size.x  > other.pos.x  &&
      obj.pos.y < other.pos.y + other.size.y &&
      obj.pos.y + obj.size.y  > other.pos.y
   );
}

bool Rectangle::CheckCollision(const Rectangle& obj, const Triangle& other) {
   return true;
}

void Rectangle::immediateDraw(const Rectangle& obj, Color color) {
   Color normal = color.normalized();
   api_set_color(global_ctx, normal.r, normal.g, normal.b, normal.a);
   api_draw_rectangle(global_ctx, obj.pos.x, obj.pos.y, obj.size.x, obj.size.y);
}

// Circle
Circle::Circle() : pos({0, 0}), radius(0) {}
Circle::Circle(float2 pos, float radius) : pos(pos), radius(radius) {}

bool Circle::CheckCollisionPoint(const Circle& obj, float2 point) {
   float dx = point.x - obj.pos.x;
   float dy = point.y - obj.pos.y;
   float distance_sq = dx * dx + dy * dy; return ( (distance_sq < obj.radius) | (fabs(distance_sq - obj.radius) < 1e-9) );
}

bool Circle::CheckCollision(const Circle& obj, const Circle& other) {
   return true;
}

bool Circle::CheckCollision(const Circle& obj, const Triangle& other) {
   return true;
}

bool Circle::CheckCollision(const Circle& obj, const Rectangle& other) {
   return true;
}

void Circle::immediateDraw(const Circle& obj, Color color) {
   Color normal = color.normalized();
   api_set_color(global_ctx, normal.r, normal.g, normal.b, normal.a);
   float data[obj.vertexCount()*3];
   api_draw_circle(global_ctx, data, obj.pos.x, obj.pos.y, obj.radius, obj.vertexCount());
}

}

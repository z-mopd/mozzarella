#include "shape.hpp"
#include "_vertex.hpp"
#include <cmath>
#include <iostream>

namespace mzr {


// Triangle

Triangle::Triangle() : p1({0, 0}), p2({0, 0}), p3({0, 0}) {}
Triangle::Triangle(float2 p1, float2 p2, float2 p3) : p1(p1), p2(p2), p3(p3) {}

std::size_t Triangle::_count() const {return 3;}

void Triangle::GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const {
   float2 adjusted_p1 = project_point_to_screen(this->p1, winsize);
   float2 adjusted_p2 = project_point_to_screen(this->p2, winsize);
   float2 adjusted_p3 = project_point_to_screen(this->p3, winsize);

   gen_triangle_direct(vertex_buffer, 7, adjusted_p1, adjusted_p2, adjusted_p3);
   map_color_ntimes(vertex_buffer, buffer_nsize(3), 3, color.normalized(), 3);
}

bool Triangle::CheckCollisionPoint(float2 point) const {
    double areaOrig = fabs((this->p2.x - this->p1.x) * (this->p3.y - this->p1.y) - (this->p3.x - this->p1.x) * (this->p2.y - this->p1.y));
    double area1 = fabs((this->p1.x - point.x) * (this->p2.y - point.y) - (this->p2.x - point.x) * (this->p1.y - point.y));
    double area2 = fabs((this->p2.x - point.x) * (this->p3.y - point.y) - (this->p3.x - point.x) * (this->p2.y - point.y));
    double area3 = fabs((this->p3.x - point.x) * (this->p1.y - point.y) - (this->p1.x - point.x) * (this->p3.y - point.y));
    return fabs(area1 + area2 + area3 - areaOrig) < 1e-6;
}


// Rectangle

Rectangle::Rectangle() : pos({0, 0}), size({0, 0}) {}
Rectangle::Rectangle(float2 pos, float2 size) : pos(pos), size(size) {}
Rectangle::Rectangle(float x, float y, float width, float height) : pos{x, y}, size{width, height} {}

std::size_t Rectangle::_count() const {return 6;}

void Rectangle::GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const {
   float2 adjusted_pos = project_point_to_screen(pos, winsize);
   float2 adjusted_size = project_size_to_screen(size, winsize);

   gen_rectangle_vert(vertex_buffer, 7, adjusted_pos, adjusted_size);
   map_color_ntimes(vertex_buffer, buffer_nsize(6), 6, color.normalized(), 3);
}

bool Rectangle::CheckCollisionPoint(float2 point) const {
   float min_x = this->pos.x;
   float min_y = this->pos.y;

   float max_x = this->pos.x + this->size.x;
   float max_y = this->pos.y + this->size.y;

   return (min_x <= point.x && point.x <= max_x && min_y <= point.y && point.y <= max_y);
}


// Circle

Circle::Circle() : pos({0, 0}), radius(0), SEGMENTS(CIRCLE_DEFAULT_SEGMENTS) {}
Circle::Circle(float2 pos, float radius) : pos(pos), radius(radius), SEGMENTS(CIRCLE_DEFAULT_SEGMENTS) {}
Circle::Circle(float2 pos, float radius, std::size_t segments) : pos(pos), radius(radius), SEGMENTS(segments) {}

std::size_t Circle::_count() const {return 0;}

void Circle::GenerateVertices(float* vertex_buffer, int2 winsize, Color color) const {}

bool Circle::CheckCollisionPoint(float2 point) const {
   float dx = point.x - this->pos.x;
   float dy = point.y - this->pos.y;
   float distance_sq = dx * dx + dy * dy;

   return ( (distance_sq < this->radius) | (fabs(distance_sq - this->radius) < 1e-9) );
}

}

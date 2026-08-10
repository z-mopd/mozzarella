#include "shape.hpp"
#include "_common.hpp"
#include "_vertex.hpp"

#include <cmath>

namespace mzr {

std::uint32_t Shape::get_bid() const {
   return this->_vbo;
}

std::uint32_t Shape::get_sid() const {
   return this->_vao;
}

Triangle::Triangle(float2 p1, float2 p2, float2 p3) : p1(p1), p2(p2), p3(p3) {
   alloc_shape_buffer(GLfloat, _init, 3);

   glGenVertexArrays(1, &this->_vao);
   glGenBuffers(1, &this->_vbo);

   glBindVertexArray(this->_vao);

   glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
   glBufferData(GL_ARRAY_BUFFER, buffer_size(GLfloat, 3), _init, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(9 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

bool Triangle::CheckCollisionPoint(float2 point) {
    double areaOrig = fabs((this->p2.x - this->p1.x) * (this->p3.y - this->p1.y) - (this->p3.x - this->p1.x) * (this->p2.y - this->p1.y));
    double area1 = fabs((this->p1.x - point.x) * (this->p2.y - point.y) - (this->p2.x - point.x) * (this->p1.y - point.y));
    double area2 = fabs((this->p2.x - point.x) * (this->p3.y - point.y) - (this->p3.x - point.x) * (this->p2.y - point.y));
    double area3 = fabs((this->p3.x - point.x) * (this->p1.y - point.y) - (this->p1.x - point.x) * (this->p3.y - point.y));
    return fabs(area1 + area2 + area3 - areaOrig) < 1e-6;
}

/*
void Triangle::_update_buffer(float2 win_size) {
   GLfloat2 win_sizef = {static_cast<GLfloat>(win_size.x), static_cast<GLfloat>(win_size.y)};
   GLfloat2 adjusted_p1 = project_point_to_screen(this->p1, win_sizef);
   GLfloat2 adjusted_p2 = project_point_to_screen(this->p2, win_sizef);
   GLfloat2 adjusted_p3 = project_point_to_screen(this->p3, win_sizef);

   gen_triangle_direct(this->_vertices.data(), 3, adjusted_p1, adjusted_p2, adjusted_p3);

   glBindVertexArray(this->_vao);
   glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
   glBufferSubData(GL_ARRAY_BUFFER, 0, this->_vertices.size() * sizeof(GLfloat), this->_vertices.data());
}

void Triangle::_set_color(Color color) {
   Color normalized_color = color.normalized();
   map_color_to_vertices(this->_vertices.data() + 3, 4, normalized_color);
}

Rectangle::Rectangle(GLfloat2 pos, GLfloat2 size) : pos(pos), size(size) {
   this->_count = 6;

   glGenVertexArrays(1, &this->_vao);
   glGenBuffers(1, &this->_vbo);

   glBindVertexArray(this->_vao);

   glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
   glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(GLfloat), this->_vertices.data(), GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}*/

Rectangle::Rectangle(float2 pos, float2 size) : pos(pos), size(size) {
   alloc_shape_buffer(GLfloat, _init, 6);

   glGenVertexArrays(1, &this->_vao);
   glGenBuffers(1, &this->_vbo);

   glBindVertexArray(this->_vao);

   glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
   glBufferData(GL_ARRAY_BUFFER, buffer_size(GLfloat, 6), _init, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(18 * sizeof(float)));
   glEnableVertexAttribArray(1);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

bool Rectangle::CheckCollisionPoint(float2 point) {
   float min_x = this->pos.x;
   float min_y = this->pos.y;

   float max_x = this->pos.x + this->size.x;
   float max_y = this->pos.y + this->size.y;

   return (min_x <= point.x && point.x <= max_x && min_y <= point.y && point.y <= max_y);
}

Circle::Circle(float2 pos, float radius) : pos(pos) {
   //TODO: implement this
}

bool Circle::CheckCollisionPoint(float2 point) {
   float dx = point.x - this->pos.x;
   float dy = point.y - this->pos.y;
   float distance_sq = dx * dx + dy * dy;

   return ( (distance_sq < this->radius) | (fabs(distance_sq - this->radius) < 1e-9) );
}

}

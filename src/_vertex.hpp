#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "_common.hpp"
#include "color.hpp"
#include <cstddef>

#define alloc_shape_buffer(type, buffer_name, points) type buffer_name[(points)*3 + (points)*4] = {}
#define buffer_size(type, points) ( ((points)*3 + (points)*4) * sizeof(type) )

void points_to_vertices(GLfloat2* point_buffer, size_t npoints, GLfloat* vertex_buffer, size_t vertex_span);
void vertices_to_points(GLfloat* vertex_buffer, size_t nbuffer, size_t vertex_span, GLfloat2* point_buffer);

void gen_triangle_direct(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos1, GLfloat2 pos2, GLfloat2 pos3);
void gen_triangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset1, GLfloat2 offset2);
void gen_triangle_degrees(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset1, GLfloat2 offset2, float degrees);

void gen_rhtriangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset);
void gen_rhtriangle_degrees(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset, float degrees);

void gen_eqtriangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat side_length);
void gen_rectangle_vert(GLfloat* vertices, size_t vertex_span, GLfloat2 pos, GLfloat2 size);

void set_vertices_to(GLfloat* vertex_buffer, size_t nbuffer, size_t vertex_span, size_t offset, float value);

void map_color_to_vertices(GLfloat* vertex_buffer, size_t nbuffer, mzr::Color color);
void map_color_ntimes(GLfloat* vertex_buffer, size_t n, mzr::Color color);

GLfloat2 project_point_to_screen(GLfloat2 p1, GLfloat2 screen_size);
GLfloat2 project_size_to_screen(GLfloat2 size, GLfloat2 screen_size);

#endif

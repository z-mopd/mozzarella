#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "_common.hpp"
#include "data_types.hpp"
#include "color.hpp"
#include <cstddef>

#define buffer_nsize(points) ((points)*3 + (points)*4)
#define buffer_byte_size(type, points) ( buffer_nsize(points) * sizeof(type) )
#define alloc_shape_buffer(type, buffer_name, points) type buffer_name[buffer_nsize(points)] = {}

using namespace mzr;

void points_to_vertices(float2* point_buffer, size_t npoints, float* vertex_buffer, size_t vertex_span);
void vertices_to_points(float* vertex_buffer, size_t nbuffer, size_t vertex_span, float2* point_buffer);

void gen_triangle_direct(float* vertex_buffer, size_t vertex_span, float2 pos1, float2 pos2, float2 pos3);
void gen_triangle_vert(float* vertex_buffer, size_t vertex_span, float2 pos, float2 offset1, float2 offset2);
void gen_triangle_degrees(float* vertex_buffer, size_t vertex_span, float2 pos, float2 offset1, float2 offset2, float degrees);

void gen_rhtriangle_vert(float* vertex_buffer, size_t vertex_span, float2 pos, float2 offset);
void gen_rhtriangle_degrees(float* vertex_buffer, size_t vertex_span, float2 pos, float2 offset, float degrees);

void gen_eqtriangle_vert(float* vertex_buffer, size_t vertex_span, float2 pos, float side_length);
void gen_rectangle_vert(float* vertices, size_t vertex_span, float2 pos, float2 size);

void gen_circle_vert(float* vertex_buffer, size_t vertex_span, float2 pos, float radius);

void set_vertices_to(float* vertex_buffer, size_t nbuffer, size_t vertex_span, size_t offset, float value);

void map_color_to_vertices(float* vertex_buffer, size_t nbuffer, Color color);
void map_color_ntimes(float* vertex_buffer, size_t nbuffer, size_t n, Color color, size_t offset);

float2 project_point_to_screen(float2 p1, int2 screen_size);
float2 project_size_to_screen(float2 size, int2 screen_size);

#endif

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <cstddef>
#include <glad/glad.h>

struct int2 {
   int x;
   int y;
};

struct int3 {
   int x;
   int y;
   int z;
};

struct float2 {
   GLfloat x;
   GLfloat y;
};

struct float3 {
   GLfloat x;
   GLfloat y;
   GLfloat z;
};

void points_to_vertices(float2* point_buffer, size_t npoints, GLfloat* vertex_buffer, size_t vertex_span);
void vertices_to_points(GLfloat* vertex_buffer, size_t nbuffer, size_t vertex_span, float2* point_buffer);

void gen_triangle_vert(GLfloat* vertex_buffer, size_t vertex_span, float2 pos, float2 offset1, float2 offset2);
void gen_triangle_degrees(GLfloat* vertex_buffer, size_t vertex_span, float2 pos, float2 offset1, float2 offset2, float degrees);

void gen_rhtriangle_vert(GLfloat* vertex_buffer, size_t vertex_span, float2 pos, float2 offset);
void gen_rhtriangle_degrees(GLfloat* vertex_buffer, size_t vertex_span, float2 pos, float2 offset, float degrees);

void gen_eqtriangle_vert(GLfloat* vertex_buffer, size_t vertex_span, float2 pos, GLfloat side_length);
void gen_rectangle_vert(GLfloat* vertices, size_t vertex_span, float2 pos, float2 size);

#endif

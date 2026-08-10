#include "_vertex.hpp"
#include "_common.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>

#define SPAN_ASSERT(condition)   assert(((void)"Vertex span cannot be below threshold!", condition))
#define BUFFER_ASSERT(condition) assert(((void)"Buffer does not meet condition!", condition))

/*
   A point is a simple location on the screen (minimally represented as two floats for regular two dimensional graphics)
   Points should be used for shapes, coordinates, and regular user-facing code
   A vertex is location on top of extra information to allow for rendering
   Vertices should only be used internally, almost never exposed to the user
*/

// Vertices are guaranteed to be ordered like so
// x1, y1, ?, ?, ...
// x2, y2, ?, ?, ...

double to_radians(double degrees) {
   return (M_PI/180)*degrees;
}

double to_degrees(double radians) {
   return (180/M_PI)*radians;
}

GLfloat2 rotate_point(GLfloat2 origin, GLfloat2 point, float degrees) {
   float px = point.x - origin.x;
   float py = point.y - origin.y;
   double radians = to_radians(degrees);

   double cos_r = cos(radians);
   double sin_r = sin(radians);

   double x_rotated = px * cos_r - py * sin_r;
   double y_rotated = px * sin_r + py * cos_r;

   return (GLfloat2){
      float(x_rotated + origin.x),
      float(y_rotated + origin.y)
   };
}

void points_to_vertices(GLfloat2* point_buffer, size_t npoints, GLfloat* vertex_buffer, size_t vertex_span) {
   SPAN_ASSERT(vertex_span >= 2); // if you reach this... what are you doing exactly?

   size_t vert_inoffset = 0;
   for (size_t i = 0; i < npoints; ++i) {
      vertex_buffer[vert_inoffset] = point_buffer[i].x;
      vertex_buffer[vert_inoffset + 1] = point_buffer[i].y;

      vert_inoffset += vertex_span;
   }
}

void vertices_to_points(GLfloat* vertex_buffer, size_t nbuffer, size_t vertex_span, GLfloat2* point_buffer) {
   SPAN_ASSERT(vertex_span >= 2);
   BUFFER_ASSERT(nbuffer % vertex_span == 0);

   size_t point_inoffset = 0;
   for (size_t i = 0; i < nbuffer; i += vertex_span) {
      point_buffer[point_inoffset] = (GLfloat2){vertex_buffer[i], vertex_buffer[i + 1]};
      ++point_inoffset;
   }
}

void gen_triangle_direct(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos1, GLfloat2 pos2, GLfloat2 pos3) {
   SPAN_ASSERT(vertex_span >= 2);

   *(vertex_buffer) = pos1.x;
   *(vertex_buffer + 1) = pos1.y;
   vertex_buffer += vertex_span;

   *(vertex_buffer) = pos2.x;
   *(vertex_buffer + 1) = pos2.y;
   vertex_buffer += vertex_span;

   *(vertex_buffer) = pos3.x;
   *(vertex_buffer + 1) = pos3.y;
}

void gen_triangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset1, GLfloat2 offset2) {
   SPAN_ASSERT(vertex_span >= 2);

   *(vertex_buffer) = pos.x;
   *(vertex_buffer + 1) = pos.y;
   vertex_buffer += vertex_span;

   *(vertex_buffer) = pos.x + offset1.x;
   *(vertex_buffer + 1) = pos.y + offset1.y;
   vertex_buffer += vertex_span;

   *(vertex_buffer) = pos.x + offset2.x;
   *(vertex_buffer + 1) = pos.y + offset2.y;
}

void gen_triangle_degrees(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset1, GLfloat2 offset2, float degrees) {
   gen_triangle_direct(vertex_buffer, vertex_span, pos,
      rotate_point(pos, offset1, degrees),
      rotate_point(pos, offset2, degrees)
   );
}

void gen_rhtriangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset) {
   gen_triangle_vert(vertex_buffer, vertex_span, pos,
      (GLfloat2){0, offset.y},
      (GLfloat2){offset.x, 0}
   );
}

void gen_rhtriangle_degrees(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 offset, float degrees) {
   gen_triangle_direct(vertex_buffer, vertex_span, pos,
      rotate_point(pos, (GLfloat2){pos.x, pos.y + offset.y}, degrees),
      rotate_point(pos, (GLfloat2){pos.x + offset.x, pos.y}, degrees)
   );
}

void gen_eqtriangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat side_length) {
   float height = side_length * sqrt(3) / 2;
   gen_triangle_vert(
      vertex_buffer, vertex_span, pos,
      (GLfloat2){-(side_length / 2), -height},
      (GLfloat2){side_length / 2, -height}
   );
}

void gen_rectangle_vert(GLfloat* vertex_buffer, size_t vertex_span, GLfloat2 pos, GLfloat2 size) {
   // Shouldn't be possible unless something upstream messed up...
   // But really, an exception here would be annoying to handle
   assert(((void)"Size cannot be negative", size.x >= 0 && size.y >= 0));

   // triangle 1
   gen_rhtriangle_vert(vertex_buffer, vertex_span, pos, (GLfloat2){size.x, -size.y});

   // triangle 2
   gen_rhtriangle_vert(
      vertex_buffer + vertex_span * 3, vertex_span,
      (GLfloat2){pos.x + size.x, pos.y - size.y},
      (GLfloat2){-size.x, size.y}
   );
}

void set_vertices_to(GLfloat* vertex_buffer, size_t nbuffer, size_t vertex_span, size_t offset, float value) {
   for (size_t i = 0; i < nbuffer; i += vertex_span) {
      vertex_buffer[i + offset] = value;
   }
}

void map_color_to_vertices(GLfloat* vertex_buffer, size_t nbuffer, mzr::Color color) {
   vertex_buffer[0] = color.r;
   vertex_buffer[1] = color.g;
   vertex_buffer[2] = color.b;
   vertex_buffer[3] = color.a;
}

void map_color_ntimes(GLfloat *vertex_buffer, size_t n, mzr::Color color) {
   for (size_t i = 0; i < n; ++i) {
      map_color_to_vertices(vertex_buffer + (i*4), n, color);
   }
}

GLfloat2 project_point_to_screen(GLfloat2 p1, GLfloat2 screen_size) {
   return (GLfloat2){(p1.x*2) / screen_size.x - 1, (p1.y*2) / (-screen_size.y) + 1};
}

GLfloat2 project_size_to_screen(GLfloat2 size, GLfloat2 screen_size) {
   return (GLfloat2){(size.x*2)/screen_size.x, (size.y*2)/screen_size.y};
}

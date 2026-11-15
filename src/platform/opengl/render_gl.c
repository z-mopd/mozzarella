#include "gl_def.h"
#include <math.h>

#define TRIANGLE_BUFFER_SIZE (9)

void api_set_projection(MzrApiCtx ctx, float* mat4x4) {
   glUniformMatrix4fv(glGetUniformLocation(ctx->current_program, "projection"), 1, GL_FALSE, mat4x4);
}

void api_set_color(MzrApiCtx ctx, float r, float g, float b, float a) {
   glUniform4f(glGetUniformLocation(ctx->current_program, "color"), r, g, b, a);
}

void api_draw_triangle(
   MzrApiCtx ctx,
   float x1, float y1,
   float x2, float y2,
   float x3, float y3
) {
   float arr[TRIANGLE_BUFFER_SIZE] = {
      x1, y1, 0,
      x2, y2, 0,
      x3, y3, 0,
   };

   glBindBuffer(GL_ARRAY_BUFFER, ctx->immediate_vbo);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arr), arr);

   glDrawArrays(GL_TRIANGLES, 0, 3);
}

size_t api_write_triangle(
   MzrApiCtx ctx,
   MzrBuffer buffer,
   size_t byteOffset,
   float x1, float y1,
   float x2, float y2,
   float x3, float y3
) {
   if (buffer->size + byteOffset + TRIANGLE_BUFFER_SIZE > buffer->size) {
      return 0;
   }

   float arr[TRIANGLE_BUFFER_SIZE] = {
      x1, y1, 0,
      x2, y2, 0,
      x3, y3, 0,
   };

   glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
   glBufferSubData(GL_ARRAY_BUFFER, byteOffset, TRIANGLE_BUFFER_SIZE, arr);

   return TRIANGLE_BUFFER_SIZE;
}

void api_draw_rectangle(MzrApiCtx ctx, float x, float y, float width, float height) {
   float arr[TRIANGLE_BUFFER_SIZE*2] = {
      x, y, 0,
      x, y+height, 0,
      x+width, y, 0,

      x+width, y+height, 0,
      x, y+height, 0,
      x+width, y, 0
   };

   glBindBuffer(GL_ARRAY_BUFFER, ctx->immediate_vbo);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arr), arr);

   glDrawArrays(GL_TRIANGLES, 0, 6);
}

void api_draw_circle(MzrApiCtx ctx, float* vertex_buffer, float xpos, float ypos, float rad, float segments) {
	float theta = 2 * M_PI / segments;
	float c = cosf(theta); // precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = rad; // we start at angle = 0
   float y = 0;

	for(size_t i = 0; i < segments; ++i) {
      vertex_buffer[i*3+0] = xpos + x;
      vertex_buffer[i*3+1] = ypos + y;
      vertex_buffer[i*3+2] = 0;

		// apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}

   glBindBuffer(GL_ARRAY_BUFFER, ctx->immediate_vbo);
   glBufferSubData(GL_ARRAY_BUFFER, 0, segments*3*sizeof(float), vertex_buffer);

   glDrawArrays(GL_TRIANGLE_FAN, 0, segments);
}

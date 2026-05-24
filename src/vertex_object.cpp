#include "vertex_object.hpp"

void vertex_object_construct(vertex_object &object, GLsizei vbo_n, GLsizei vao_n) {
   glGenVertexArrays(vao_n, &object.vao);
   glGenBuffers(vbo_n, &object.vbo);

   glBindVertexArray(object.vao);
   glBindBuffer(GL_ARRAY_BUFFER, object.vbo);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

#ifndef VERTEX_OBJECT_HPP
#define VERTEX_OBJECT_HPP

#include <glad/glad.h>

struct vertex_object {
   GLuint vbo;
   GLuint vao;
};

struct vertex_attrib {
   GLuint start;
   GLint size;
   GLenum type;
};

void vertex_object_construct(vertex_object& object, GLsizei vbo_n, GLsizei vao_n);

#endif

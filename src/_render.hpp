#ifndef RENDER_HPP
#define RENDER_HPP

#include "_common.hpp"
#include "events.hpp"
#include <cstddef>

bool r_init();

void mzr::ProcessEvents();
GLuint query_shader_program();
GLFWwindow* first_context();

void submit_geometry(GLenum draw_mode, GLfloat *buffer, size_t buffer_size, size_t count);

void batch_geometry(GLenum draw_mode);

void draw(); // execute top of stack

void flush(); // execute everything on stack


void r_close();

#endif

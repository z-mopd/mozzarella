#ifndef RENDER_HPP
#define RENDER_HPP

#include "_common.hpp"
#include "events.hpp"
#include <cstddef>

bool r_init();

void mzr::ProcessEvents();
GLuint query_shader_program();
GLFWwindow* first_context();

void update_vertex_attrib();

void submit_immediate_call(GLenum draw_mode, float* buffer, size_t buffer_size, size_t count);

void draw(); // execute top of stack
void draw_static(GLenum draw_mode, GLuint vbo, std::size_t count);

void flush(); // execute everything on stack


void r_close();

#endif

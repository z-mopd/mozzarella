#ifndef RENDER_HPP
#define RENDER_HPP

#include "_common.hpp"
#include "events.hpp"
#include "shape.hpp"

bool r_init();

void mzr::ProcessEvents();
GLuint query_shader_program();
GLFWwindow* first_context();

//TODO: write up a renderer

void r_close();

#endif

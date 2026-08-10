#include "_render.hpp"

#include <cstdarg>
#include <iostream>

static GLFWwindow* _first_context = nullptr;
static GLuint shader_program;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 color;\n"
"out vec4 vertColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertColor = color;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertColor;\n"
"}\n\0";

bool r_init() {
   if (!glfwInit()) {
      std::cout << "Could not start glfw\n";
      return false;
   }

   glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
   _first_context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
   if (!_first_context) {
      glfwTerminate();
      return false;
   }

   glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

   glfwMakeContextCurrent(_first_context);
   gladLoadGL();
   glViewport(0, 0, 1, 1);

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
   glCompileShader(vertexShader);

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
   glCompileShader(fragmentShader);

   shader_program = glCreateProgram();

   glAttachShader(shader_program, vertexShader);
   glAttachShader(shader_program, fragmentShader);
   glLinkProgram(shader_program);

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   return true;
}

void mzr::ProcessEvents() {
   glfwPollEvents();
}

GLuint query_shader_program() {
   return shader_program;
}

GLFWwindow* first_context() {
   return _first_context;
}

void r_close() {
   glDeleteProgram(shader_program);
   glfwDestroyWindow(_first_context);
   glfwTerminate();
}

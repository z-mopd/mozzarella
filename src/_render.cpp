#include "_render.hpp"
#include "_common.hpp"

#include <iostream>

static GLFWwindow* _first_context = nullptr;
static GLuint shader_program;

static const std::size_t STACK_SIZE = 2<<12;

static GLuint shared_vao;
static GLuint shared_vbo;

static GLenum modes[STACK_SIZE];
static std::size_t counts[STACK_SIZE];
static std::size_t byte_offset[STACK_SIZE] = {};

static std::size_t stack_index = 0;

static std::size_t total_vert = 0;
static std::size_t total_offset = 0;

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
      std::cerr << "Could not start glfw!\n";
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

   glGenVertexArrays(1, &shared_vao);
   glGenBuffers(1, &shared_vbo);

   glBindVertexArray(shared_vao);
   glBindBuffer(GL_ARRAY_BUFFER, shared_vbo);
   glBufferData(GL_ARRAY_BUFFER, STACK_SIZE * 7 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

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

void update_vertex_attrib() {
   glBindBuffer(GL_ARRAY_BUFFER, shared_vbo);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
}

void submit_immediate_call(GLenum draw_mode, GLfloat* buffer, std::size_t buffer_size, std::size_t count) {
   modes[stack_index] = draw_mode;
   counts[stack_index] = count;
   byte_offset[stack_index] = buffer_size;

   glBindBuffer(GL_ARRAY_BUFFER, shared_vbo);
   glBufferSubData(GL_ARRAY_BUFFER, total_offset, buffer_size, buffer);

   total_offset += buffer_size;
   total_vert += count;
   ++stack_index;
}

void draw() {
   if (stack_index == 0) return;
   --stack_index;

   total_offset -= byte_offset[stack_index];
   total_vert -= counts[stack_index];

   glBindBuffer(GL_ARRAY_BUFFER, shared_vbo);
   glDrawArrays(modes[stack_index], total_vert, counts[stack_index]);
}

void draw_static(GLenum draw_mode, GLuint vbo, std::size_t count) {
   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glDrawArrays(draw_mode, 0, count);
}

void flush() {
   while (stack_index > 0) {
      draw();
   }
}

void r_close() {
   glDeleteProgram(shader_program);
   glfwDestroyWindow(_first_context);
   glfwTerminate();
}

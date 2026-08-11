#include "_render.hpp"
#include "_common.hpp"

#include <iostream>

static GLFWwindow* _first_context = nullptr;
static GLuint shader_program;

static const std::size_t STACK_SIZE = 2<<12;

static GLenum modes[STACK_SIZE] = {};

static GLuint shared_vao;

static GLuint vbos[STACK_SIZE];
static std::size_t counts[STACK_SIZE] = {};

static std::size_t top_index = 0;

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
   glGenBuffers(STACK_SIZE, vbos);

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

void submit_geometry(GLenum draw_mode, GLfloat *buffer, std::size_t buffer_size, std::size_t count) {
   modes[top_index] = draw_mode;
   counts[top_index] = count;

   GLuint vbo = vbos[top_index];

   glBindBuffer(GL_ARRAY_BUFFER, vbo);
   glBufferData(GL_ARRAY_BUFFER, buffer_size, buffer, GL_STREAM_DRAW);

   ++top_index;
}

// TODO: implement this
void batch_geometry(GLenum draw_mode) {}

void draw() {
   if (top_index == 0) return;
   --top_index;

   glBindVertexArray(shared_vao);
   glBindBuffer(GL_ARRAY_BUFFER, vbos[top_index]);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
   glEnableVertexAttribArray(1);

   glDrawArrays(modes[top_index], 0, counts[top_index]);
}

void flush() {
   while (top_index > 0) {
      draw();
   }
}

void r_close() {
   glDeleteProgram(shader_program);
   glfwDestroyWindow(_first_context);
   glfwTerminate();
}

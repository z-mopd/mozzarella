#include "vertex.hpp"
#include "GLFW/glfw3.h"
#include "render.hpp"

#include <iostream>
#include <cmath>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() {
   r_init();

	GLfloat vertices[] = {
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

   GLfloat squareVertices[] = {
      0.0f, 0.5f, 0.0f,
      -1.0f, -0.5f, 0.0f,
      -1.0f, 0.5f, 0.0f,

      -1.0f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f,
      0.0f, -0.5f, 0.0f,
   };

   GLfloat triangleVertices[9] = {};
   gen_rhtriangle_degrees(triangleVertices, 3, {-0.5f, 0}, {1.0f, 1.0f}, -45);
   //gen_rhtriangle_vert(triangleVertices, 3, {-0.5, -0.5}, {1, 1});

   for (size_t i = 0; i < 9; i += 3) {
      std::cout << "(" << triangleVertices[i] << ", " << triangleVertices[i + 1] << ")\n";
   }

   GLfloat generatedVertices[18];
   gen_rectangle_vert(generatedVertices, 3, {-1.0f, 0.5f}, {1.0f, 1.0f});

   int winWidth = 640;
   int winHeight = 480;

   GLFWwindow* window;
   window = glfwCreateWindow(winWidth, winHeight, "this is a window", nullptr, nullptr);

   glfwMakeContextCurrent(window);
   glViewport(0, 0, winWidth, winHeight);

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
   glCompileShader(vertexShader);

   GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
   glCompileShader(fragmentShader);

   GLuint shaderProgram = glCreateProgram();

   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   GLuint VAO, VBO;

   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   glBindVertexArray(VAO);

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   while (!glfwWindowShouldClose(window)) {
      glUseProgram(shaderProgram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glfwSwapBuffers(window);

      glfwPollEvents();
   }

   /*
   Window window;
   window_hints(2, GLFW_RESIZABLE, GLFW_FALSE);
   window_construct(&window, {0, 0}, {winWidth, winHeight}, "this is a window");
   */

   std::cout << "window created\n";

   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteProgram(shaderProgram);
   glfwDestroyWindow(window);

   r_close();

   return 0;
}

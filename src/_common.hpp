#ifndef _COMMON_HPP
#define _COMMON_HPP

// header for commonly included headers and frequently used types

#include "external/glad.h"
#include "external/glfw/include/GLFW/glfw3.h"

struct GLint2 {
   GLint x;
   GLint y;
};

struct GLint3 {
   GLint x;
   GLint y;
   GLint z;
};

struct GLfloat2 {
   GLfloat x;
   GLfloat y;
};

struct GLfloat3 {
   GLfloat x;
   GLfloat y;
   GLfloat z;
};

struct GLfloat4 {
   GLfloat x;
   GLfloat y;
   GLfloat z;
   GLfloat w;
};

#endif

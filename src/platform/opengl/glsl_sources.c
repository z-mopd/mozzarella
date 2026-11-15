#include "gl_def.h"

// shape sources
const char* shape2DVert = ""
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"  gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}";

const char* shape2DFrag = ""
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main() {\n"
"   FragColor = color;\n"
"}";


// text sources
const char* textVert = ""
"#version 330 core\n"
"layout (location = 0) in vec4 vertex;"
"out vec2 TexCoords;"

"uniform mat4 projection;"

"void main() {"
"    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);"
"    TexCoords = vertex.zw;"
"}";

const char* textFrag = ""
"#version 330 core\n"
"in vec2 TexCoords;"

"uniform sampler2D text;"
"uniform vec4 color;"

"void main() {"
"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
"    FragColor = color * sampled;"
"}";

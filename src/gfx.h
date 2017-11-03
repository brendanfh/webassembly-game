#ifndef __GFX_H_
#define __GFX_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

namespace Gfx {
	extern GLuint program;
    extern GLuint vbo;

    extern GLuint vertShader;
    extern GLuint fragShader;

    extern GLFWwindow* window;

    void Initialize();
    GLuint CompileShader(GLenum shaderType, const char *shaderSrc);
    GLuint CreateProgram(GLuint vertShader, GLuint fragShader);

    void BufferData(GLsizeiptr size, const GLfloat* data);
    void SetupDraw();
    void CleanupDraw();
};

#endif
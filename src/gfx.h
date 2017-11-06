#ifndef __GFX_H_
#define __GFX_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

namespace Gfx {
	extern GLuint program;
    extern GLuint vbo;
    extern GLuint ibo;

    extern GLuint vertShader;
    extern GLuint fragShader;

    extern GLFWwindow* window;

    void Initialize();
    GLuint CompileShader(GLenum shaderType, const char *shaderSrc);
    GLuint CreateProgram(GLuint vertShader, GLuint fragShader);

    void BufferData(GLintptr offset, GLsizeiptr size, const GLfloat* data);
    void SetupDraw();
    void CleanupDraw();

    class Quad {
    private:
        int id;
        GLfloat* data;

    public:
        Quad(int id);
        ~Quad();
        void SetRect(float x, float y, float w, float h);
        void SetColor(float r, float g, float b, float a);
        void BufferData();
    };
};

#endif

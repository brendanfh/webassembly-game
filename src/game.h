#ifndef __GAME_H_
#define __GAME_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

GLuint CompileShader(GLenum shaderType, const char *shaderSrc);
GLuint CreateProgram(GLuint vertShader, GLuint fragShader);

class Game {
public:
    Game();
    ~Game();

    void tick();
    void render();

private:
    GLFWwindow* window;

	GLuint program;
    GLuint vbo;
};

#endif

#include "game.h"
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

GLuint CompileShader(GLenum shaderType, const char *shaderSrc) {
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(shaderType);
	if (shader == 0)
		return 0;

	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = new char[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			cout << "Error compiling shader: " << endl << infoLog << endl;

			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint CreateProgram(GLuint vertShader, GLuint fragShader) {
	GLuint program;
	GLint linked;

	program = glCreateProgram();
	if (program == 0)
		return 0;

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = new char[infoLen];
			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			cout << "Error linking program: " << endl << infoLog << endl;

			delete[] infoLog;
		}

		glDeleteProgram(program);
		return 0;
	}

	return program;
}

Game::Game() {
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    const GLfloat vertexData[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,     
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

	char vertShaderSrc[] =
		"attribute vec3 aPosition; \n"
		"attribute vec4 aColor;    \n"
		"varying vec4 vColor;      \n"
		"\n"
		"void main() {\n"
		"   vColor = aColor;"
		"	gl_Position = vec4(aPosition, 1.0);\n"
		"}\n";

	char fragShaderSrc[] =
		"precision mediump float; \n"
		"varying vec4 vColor; \n"
		"\n"
		"void main() {\n"
		"	gl_FragColor = vColor;\n"
		"}\n";

	GLuint vertShader = CompileShader(GL_VERTEX_SHADER, vertShaderSrc);
	GLuint fragShader = CompileShader(GL_FRAGMENT_SHADER, fragShaderSrc);

	program = CreateProgram(vertShader, fragShader);

	glBindAttribLocation(program, 0, "aPosition");
	glBindAttribLocation(program, 1, "aColor");
	glUseProgram(program);
}

Game::~Game() {
    cout << "Destuctor was called" << endl;
}

void Game::tick() {
}

void Game::render() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*4, (void*)(0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*4, (void*)(4 * 3));

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

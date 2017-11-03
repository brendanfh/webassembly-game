#include "gfx.h"

//Have to declare these somewhere...
GLFWwindow* Gfx::window = NULL;
GLuint Gfx::program = -1;
GLuint Gfx::fragShader = -1;
GLuint Gfx::vertShader = -1;
GLuint Gfx::vbo = -1;

void Gfx::Initialize() {
    if (!glfwInit())
        exit(EXIT_FAILURE);
    Gfx::window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!Gfx::window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	glfwMakeContextCurrent(Gfx::window);

	glGenBuffers(1, &Gfx::vbo);

	char vertShaderSrc[] =
		"attribute vec3 aPosition; \n"
		"attribute vec4 aColor;    \n"
		"varying vec4 vColor;      \n"
		"\n"
		"void main() {\n"
		"   vColor = aColor;"
		"	gl_Position = vec4(aPosition, 1.0);\n"
		"}";

	char fragShaderSrc[] =
		"precision mediump float; \n"
		"varying vec4 vColor; \n"
		"\n"
		"void main() {\n"
		"	gl_FragColor = vColor;\n"
		"}";

	Gfx::vertShader = Gfx::CompileShader(GL_VERTEX_SHADER, vertShaderSrc);
	Gfx::fragShader = Gfx::CompileShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	
	Gfx::program = Gfx::CreateProgram(Gfx::vertShader, Gfx::fragShader);

	glBindAttribLocation(program, 0, "aPosition");
	glBindAttribLocation(program, 1, "aColor");
	glUseProgram(program);
}

GLuint Gfx::CompileShader(GLenum shaderType, const char *shaderSrc) {
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

GLuint Gfx::CreateProgram(GLuint vertShader, GLuint fragShader) {
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

void Gfx::BufferData(GLsizeiptr size, const GLfloat* data) {
    glBindBuffer(GL_ARRAY_BUFFER, Gfx::vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Gfx::SetupDraw() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, Gfx::vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*4, (void*)(0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*4, (void*)(4 * 3));
}

void Gfx::CleanupDraw() {
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
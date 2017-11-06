#include "gfx.h"

//Have to declare these somewhere...
GLFWwindow* Gfx::window = NULL;
float Gfx::width = 0.0f;
float Gfx::height = 0.0f;
float Gfx::canvasWidth = 0.0f;
float Gfx::canvasHeight = 0.0f;

//These are privated because they are not declared in the header
GLuint program = -1;
GLuint fragShader = -1;
GLuint vertShader = -1;
GLuint vbo = -1;
GLuint ibo = -1;

void SetupResize() {
    auto resizeFunc = [](GLFWwindow* window, int width, int height) {
        Gfx::canvasWidth = (float)width;
        Gfx::canvasHeight = (float)height;

        glViewport(0, 0, width, height);
    };

    glfwSetFramebufferSizeCallback(Gfx::window, resizeFunc);

    int currWidth, currHeight;
    glfwGetFramebufferSize(Gfx::window, &currWidth, &currHeight);
    resizeFunc(Gfx::window, currWidth, currHeight);
}

void Gfx::Initialize() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    Gfx::window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!Gfx::window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(Gfx::window);


    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    char vertShaderSrc[] =
        "attribute vec2 aPosition; \n"
        "attribute vec2 aTexCoord; \n"
        "attribute vec4 aColor;    \n"
        "uniform mat4 uProjection; \n"
        "\n"
        "varying vec4 vColor;      \n"
        "varying vec2 vTexCoord;   \n"
        "\n"
        "void main() {\n"
        "   vColor = aColor;"
        "   vTexCoord = aTexCoord;"
        "	gl_Position = uProjection * vec4(aPosition, 1.0, 1.0);\n"
        "}";

    char fragShaderSrc[] =
        "precision mediump float; \n"
        "uniform sampler2D uTexture; \n"
        "varying vec4 vColor; \n"
        "varying vec2 vTexCoord;   \n"
        "\n"
        "void main() {\n"
        "	gl_FragColor = texture2D(uTexture, vTexCoord) * vColor;\n"
        "}";

    vertShader = Gfx::CompileShader(GL_VERTEX_SHADER, vertShaderSrc);
    fragShader = Gfx::CompileShader(GL_FRAGMENT_SHADER, fragShaderSrc);
    
    program = Gfx::CreateProgram(vertShader, fragShader);

    glBindAttribLocation(program, 0, "aPosition");
    glBindAttribLocation(program, 1, "aTexCoord");
    glBindAttribLocation(program, 2, "aColor");
    glUseProgram(program);

    GLfloat* emptyData = new GLfloat[4 * 8 * 1024];
    for (int i = 0; i < 4 * 8 * 1024; i++)
        emptyData[i] = 0.0f;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 8 * 1024, emptyData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    GLuint* indexData = new GLuint[6 * 1024];
    for (int i = 0; i < 1024; i++) {
        indexData[i * 6 + 0] = i * 4 + 0;
        indexData[i * 6 + 1] = i * 4 + 1;
        indexData[i * 6 + 2] = i * 4 + 2;
        indexData[i * 6 + 3] = i * 4 + 0;
        indexData[i * 6 + 4] = i * 4 + 2;
        indexData[i * 6 + 5] = i * 4 + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6 * 1024, indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

    Gfx::SetSize(1.0f, 1.0f);
    SetupResize();

    GLuint tex = Gfx::LoadTexture("res/test.png");
    // glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, tex);

    GLuint texUniform = glGetUniformLocation(program, "uTexture");
    glUniform1i(texUniform, 0);
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

void Gfx::SetSize(float width, float height) {
    Gfx::width = width;
    Gfx::height = height;

    GLuint projUniform = glGetUniformLocation(program, "uProjection");

    glm::mat4x4 proj = glm::ortho(0.0f, Gfx::width, Gfx::height, 0.0f, -0.1f, -1000.0f);
    glUniformMatrix4fv(projUniform, 1, GL_FALSE, glm::value_ptr(proj));
}

void Gfx::BufferData(GLintptr offset, GLsizeiptr size, const GLfloat* data) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Gfx::SetupDraw() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8*4, (void*)(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*4, (void*)(4 * 2));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8*4, (void*)(4 * 4));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void Gfx::CleanupDraw() {
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

GLuint Gfx::LoadTexture(const char * path) {
    SDL_Surface* image;
    if (!(image = IMG_Load(path))) {
        cout << "Failed to load image: " << path << endl;
        return -1; //do better than this
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    cout << image->w << " x " << image->h << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, NULL);

    SDL_FreeSurface(image);
    return texture;
}

//Quad Class Functions
Gfx::Quad::Quad(int id) {
    this->id = id;
    this->renderData = new GLfloat[4 * 8];

    renderData[8 * 0 + 2] = 0.0f;
    renderData[8 * 0 + 3] = 1.0f;
    renderData[8 * 1 + 2] = 1.0f;
    renderData[8 * 1 + 3] = 1.0f;
    renderData[8 * 2 + 2] = 1.0f;
    renderData[8 * 2 + 3] = 0.0f;
    renderData[8 * 3 + 2] = 0.0f;
    renderData[8 * 3 + 3] = 0.0f;
}

Gfx::Quad::~Quad() {
    delete[] this->renderData;
}

void Gfx::Quad::SetRect(float x, float y, float w, float h) {
    renderData[8 * 0 + 0] = x;
    renderData[8 * 0 + 1] = y;
    renderData[8 * 1 + 0] = x + w;
    renderData[8 * 1 + 1] = y;
    renderData[8 * 2 + 0] = x + w;
    renderData[8 * 2 + 1] = y + h;
    renderData[8 * 3 + 0] = x;
    renderData[8 * 3 + 1] = y + h;
}

void Gfx::Quad::SetColor(float r, float g, float b, float a) {
    renderData[8 * 0 + 4] = r;
    renderData[8 * 0 + 5] = g;
    renderData[8 * 0 + 6] = b;
    renderData[8 * 0 + 7] = a;
    renderData[8 * 1 + 4] = r;
    renderData[8 * 1 + 5] = g;
    renderData[8 * 1 + 6] = b;
    renderData[8 * 1 + 7] = a;
    renderData[8 * 2 + 4] = r;
    renderData[8 * 2 + 5] = g;
    renderData[8 * 2 + 6] = b;
    renderData[8 * 2 + 7] = a;
    renderData[8 * 3 + 4] = r;
    renderData[8 * 3 + 5] = g;
    renderData[8 * 3 + 6] = b;
    renderData[8 * 3 + 7] = a;
}

void Gfx::Quad::BufferData() {
    Gfx::BufferData(id * 32 * sizeof(GLfloat), 32 * sizeof(GLfloat), renderData);
}
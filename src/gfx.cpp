#include "gfx.hpp"

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

GLuint texUniform = -1;
GLuint projUniform = -1;

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    projUniform = glGetUniformLocation(program, "uProjection");
    texUniform = glGetUniformLocation(program, "uTexture");

    GLfloat* emptyData = new GLfloat[4 * 8 * MAX_QUADS];
    for (int i = 0; i < 4 * 8 * MAX_QUADS; i++)
        emptyData[i] = 0.0f;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 8 * MAX_QUADS, emptyData, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);

    GLushort* indexData = new GLushort[6 * MAX_QUADS];
    for (int i = 0; i < MAX_QUADS; i++) {
        indexData[i * 6 + 0] = i * 4 + 0;
        indexData[i * 6 + 1] = i * 4 + 1;
        indexData[i * 6 + 2] = i * 4 + 2;
        indexData[i * 6 + 3] = i * 4 + 0;
        indexData[i * 6 + 4] = i * 4 + 2;
        indexData[i * 6 + 5] = i * 4 + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6 * MAX_QUADS, indexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
    
    delete[] emptyData;
    delete[] indexData;

    Gfx::SetSize(1.0f, 1.0f);
    SetupResize();

    Gfx::UseTextureUnit(0);
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

    GLfloat* orthoMat = new GLfloat[16];
    for (int i = 0; i < 16; i++) {
        orthoMat[i] = 0.0f;
    }

    orthoMat[0 * 4 + 0] = 2 / width;
    orthoMat[1 * 4 + 1] = 2 / -height;
    orthoMat[2 * 4 + 2] = -2 / (-1000.0f + 0.1f);
    orthoMat[3 * 4 + 3] = 1;

    orthoMat[3 * 4 + 0] = -1;
    orthoMat[3 * 4 + 1] = 1;
    orthoMat[3 * 4 + 2] = -(-1000.0f + -0.1f) / (-1000.0f - -0.1f);

    glUniformMatrix4fv(projUniform, 1, GL_FALSE, orthoMat);
    delete[] orthoMat;
}

void Gfx::BufferData(GLintptr offset, GLsizeiptr size, const GLfloat* data) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Gfx::ClearData(int start, int count) {
    GLfloat* empty = new GLfloat[count * 4 * 8];
    for (int i = 0; i < count * 4 * 8; i++) {
        empty[i] = 0.0f;
    }
    Gfx::BufferData(4 * 8 * start * sizeof(GLfloat), 4 * 8 * count * sizeof(GLfloat), empty);
    delete[] empty;
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

void Gfx::DrawRect(int id, float x, float y, float w, float h, float r, float g, float b, float a) {
    GLfloat* data = new GLfloat[4 * 8];
    data[0 * 8 + 0] = x;
    data[0 * 8 + 1] = y;
    data[0 * 8 + 2] = 31.0f / 32.0f;
    data[0 * 8 + 3] = 31.0f / 32.0f;
    data[0 * 8 + 4] = r;
    data[0 * 8 + 5] = g;
    data[0 * 8 + 6] = b;
    data[0 * 8 + 7] = a;
    data[1 * 8 + 0] = x + w;
    data[1 * 8 + 1] = y;
    data[1 * 8 + 2] = 1.0f;
    data[1 * 8 + 3] = 31.0f / 32.0f;
    data[1 * 8 + 4] = r;
    data[1 * 8 + 5] = g;
    data[1 * 8 + 6] = b;
    data[1 * 8 + 7] = a;
    data[2 * 8 + 0] = x + w;
    data[2 * 8 + 1] = y + h;
    data[2 * 8 + 2] = 1.0f;
    data[2 * 8 + 3] = 1.0f;
    data[2 * 8 + 4] = r;
    data[2 * 8 + 5] = g;
    data[2 * 8 + 6] = b;
    data[2 * 8 + 7] = a;
    data[3 * 8 + 0] = x;
    data[3 * 8 + 1] = y + h;
    data[3 * 8 + 2] = 31.0f / 32.0f;
    data[3 * 8 + 3] = 1.0f;
    data[3 * 8 + 4] = r;
    data[3 * 8 + 5] = g;
    data[3 * 8 + 6] = b;
    data[3 * 8 + 7] = a;
    
    Gfx::BufferData(id * 4 * 8 * sizeof(GLfloat), 4 * 8 * sizeof(GLfloat), data);
    delete[] data;
}

Gfx::Texture* Gfx::LoadTexture(const char * path) {
    SDL_Surface* image;
    if (!(image = IMG_Load(path))) {
        cout << "Failed to load image: " << path << endl;
        return new Gfx::Texture(-1, NULL); //do better than this
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, NULL);

    Gfx::Texture* texture = new Gfx::Texture(textureID, image);
    return texture;
}

void Gfx::UseTextureUnit(int tunit) {
    glUniform1i(texUniform, tunit);
}

//Quad Class Functions
Gfx::Quad::Quad(int id) {
    this->id = id;
    this->renderData = new GLfloat[4 * 8];
    for (int i = 0; i < 4 * 8; i++) {
        renderData[i] = 0.0f;
    }
    this->SetSubTexture(0, 0, 1, 1, 1, 1);
}

Gfx::Quad::~Quad() {
    delete[] this->renderData;
}

void Gfx::Quad::GetRect(float &x, float &y, float &w, float &h) {
    w = renderData[8 * 1 + 0] - renderData[8 * 0 + 0];
    h = renderData[8 * 2 + 1] - renderData[8 * 0 + 1];
    x = renderData[0];
    y = renderData[1];
}

void Gfx::Quad::SetPos(float x, float y) {
    float w = renderData[8 * 1 + 0] - renderData[8 * 0 + 0];
    float h = renderData[8 * 2 + 1] - renderData[8 * 0 + 1];

    SetRect(x, y, w, h);
}

void Gfx::Quad::SetSize(float w, float h) {
    float x = renderData[0];
    float y = renderData[1];

    SetRect(x, y, w, h);
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
    for (int i = 0; i < 4; i++) {
        renderData[8 * i + 4] = r;
        renderData[8 * i + 5] = g;
        renderData[8 * i + 6] = b;
        renderData[8 * i + 7] = a;
    }
}

void Gfx::Quad::SetSubTexture(float x, float y, float w, float h, float sw, float sh) {
    float x1 = x / sw;
    float y1 = y / sh;
    float x2 = (x + w) / sw;
    float y2 = (y + h) / sh;

    renderData[8 * 0 + 2] = x1;
    renderData[8 * 0 + 3] = y1;
    renderData[8 * 1 + 2] = x2;
    renderData[8 * 1 + 3] = y1;
    renderData[8 * 2 + 2] = x2;
    renderData[8 * 2 + 3] = y2;
    renderData[8 * 3 + 2] = x1;
    renderData[8 * 3 + 3] = y2;
}

void Gfx::Quad::BufferData() {
    Gfx::BufferData(id * 32 * sizeof(GLfloat), 32 * sizeof(GLfloat), renderData);
}

Gfx::Texture::Texture(GLuint id, SDL_Surface* image) {
    this->id = id;
    this->image = image;
    this->width = image->w;
    this->height = image->h;
}

Gfx::Texture::~Texture() {
    if (this->image != NULL)
        SDL_FreeSurface(this->image);
}

void Gfx::Texture::Use(int tunit) {
    glActiveTexture(GL_TEXTURE0 + tunit);
    glBindTexture(GL_TEXTURE_2D, this->id);
}
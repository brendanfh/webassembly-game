#ifndef __GFX_H_
#define __GFX_H_

#define MAX_QUADS 1024

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>

using namespace std;

namespace Gfx {
    class Quad {
    private:
        GLfloat* renderData;

    public:
        int id;

        Quad(int id);
        ~Quad();
        void GetRect(float &x, float &y, float &w, float &h);

        void SetPos(float x, float y);
        void SetSize(float w, float h);
        void SetRect(float x, float y, float w, float h);
        void SetColor(float r, float g, float b, float a);
        void SetSubTexture(float x, float y, float w, float h, float sw, float sh);
        void BufferData();
    };

    class Texture {
    private:
        GLuint id;
        SDL_Surface* image;

    public:
        float width;
        float height;

        Texture(GLuint id, SDL_Surface* image);
        ~Texture();
        void Use(int tuint = 0);
    };

    extern GLFWwindow* window;

    extern float width; //Width according to the game
    extern float height;
    extern float canvasWidth; //Width of the canvas element
    extern float canvasHeight;

    void Initialize();
    GLuint CompileShader(GLenum shaderType, const char *shaderSrc);
    GLuint CreateProgram(GLuint vertShader, GLuint fragShader);

    void SetSize(float width, float height);

    void BufferData(GLintptr offset, GLsizeiptr size, const GLfloat* data);
    void SetupDraw();
    void CleanupDraw();

    Gfx::Texture* LoadTexture(const char * path);
    void UseTextureUnit(int unit);
};

#endif

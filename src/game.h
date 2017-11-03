#ifndef __GAME_H_
#define __GAME_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

class Game {
private:
    GLfloat* quadData;
    double lastTime;

    double t;
public:
    Game();
    ~Game();

    void Triangle(float size, float x, float y);
    void tick();
    void render();
};

#endif

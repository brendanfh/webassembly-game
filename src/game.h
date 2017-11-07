#ifndef __GAME_H_
#define __GAME_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

#include "gfx.h"
#include "input.h"

using namespace std;

class GameTimer {
private:
    double lt;
    double dt;

public:
    GameTimer();
    ~GameTimer();
    double GetDt();
};

class Game {
private:
    GameTimer* timer;

public:
    Game();
    ~Game();

    void tick();
    void render();
};

#endif

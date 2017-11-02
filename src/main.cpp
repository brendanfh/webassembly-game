#include <GLFW/glfw3.h>
#include "emscripten.h"
#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

Game* game;
void mainLoop() {
    game->tick();
    game->render();
    glfwPollEvents();
}

int main(void) {
    game = new Game();
    emscripten_set_main_loop(mainLoop, 0, 0);

    exit(EXIT_SUCCESS);
}
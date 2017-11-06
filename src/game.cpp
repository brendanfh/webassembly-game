#include "game.h"
#include "gfx.h"

#include <cmath>
#include <string>

Game::Game() {
    Gfx::Initialize();
    lastTime = glfwGetTime();

    t = 0.0;
    quad = new Gfx::Quad(0);
    quad->SetColor(1.0f, 0.0f, 1.0f, 1.0f);
}

Game::~Game() {
    cout << "Destuctor was called" << endl;
    delete quad;
}

#define TAU (3.1415926595 * 2)
void Game::tick() {
    double currTime = glfwGetTime();
    double dt = currTime - lastTime;
    lastTime = currTime;

    t += dt;
    if (t > TAU) t -= TAU;
    quad->SetRect(cos(t) * 3.0f + 5.0f, sin(t) * 3.0f + 5.0f, 4.0f, 4.0f);
    quad->BufferData();
}

void Game::render() {
    int width, height;
    glfwGetFramebufferSize(Gfx::window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gfx::SetupDraw();
    glDrawElements(GL_TRIANGLES, 1024, GL_UNSIGNED_INT, (void*) 0);
    Gfx::CleanupDraw();
}

#include "game.h"
#include "gfx.h"

#include <cmath>
#include <string>

Game::Game() {
    Gfx::Initialize();
    Keys::Initialize();
    Mouse::Initialize();

    Gfx::SetSize(10.0f, 10.0f);

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

    // if (Keys::IsDown(GLFW_KEY_LEFT)) {
    //     t -= dt;
    // }
    // if (Keys::IsDown(GLFW_KEY_RIGHT)) {
    //     t += dt;
    // }
    // if (t > TAU) t -= TAU;
    float x = 0.0f, y = 0.0f;
    if(Mouse::IsButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        Mouse::GetScreenPos(x, y);
        quad->SetRect(x, y, 1.0f, 1.0f);
    }

    quad->BufferData();

    //Update the keyboard to clear the previous keys
    Keys::Tick();
    Mouse::Tick();
}

void Game::render() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gfx::SetupDraw();
    glDrawElements(GL_TRIANGLES, 1024, GL_UNSIGNED_INT, (void*) 0);
    Gfx::CleanupDraw();
}

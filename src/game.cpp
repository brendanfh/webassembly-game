#include "game.h"
#include "gfx.h"

#include <cmath>
#include <string>

GameTimer::GameTimer() {
    this->lt = glfwGetTime();
    this->dt = 0.0;
}

GameTimer::~GameTimer() {
    this->lt = 0.0;
    this->dt = 0.0;
}

double GameTimer::GetDt() {
    double curr = glfwGetTime();
    this->dt = curr - this->lt;
    this->lt = curr;

    return this->dt;
}

Game::Game() {
    Gfx::Initialize();
    Keys::Initialize();
    Mouse::Initialize();

    Gfx::SetSize(16.0f, 12.0f);

    timer = new GameTimer();
    world = new World();

    mainTexture = Gfx::LoadTexture("res/tilemap.png");
    mainTexture->Use(0);

    Player* ply = new Player(0);
    world->AddEntity(ply);
}

Game::~Game() {
    cout << "Destuctor was called" << endl;
    delete timer;
    delete world;
}

void Game::tick() {
    float dt = (float) timer->GetDt();

    world->Tick(dt);

    Keys::Tick();
    Mouse::Tick();
}

void Game::render() {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    world->Render();

    Gfx::SetupDraw();
    glDrawElements(GL_TRIANGLES, 1024, GL_UNSIGNED_INT, (void*) 0);
    Gfx::CleanupDraw();
}

#ifndef __GAME_H_
#define __GAME_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

#include "gfx.hpp"
#include "input.hpp"
#include "world/player.hpp"
#include "world/enemy.hpp"
#include "world/world.hpp"
#include "world/tilemap.hpp"

class GameTimer {
private:
    double lt;
    double dt;

public:
    GameTimer() {
        this->lt = glfwGetTime();
        this->dt = 0.0;
    }

    ~GameTimer() {
        this->lt = 0.0;
        this->dt = 0.0;
    }

    double GetDt() {
        double curr = glfwGetTime();
        this->dt = curr - this->lt;
        this->lt = curr;

        return this->dt;
    }
};

//Have to declare these here because GameState friends Game
class GameState;
class Game; 

class GameState {
    friend Game; 
protected:
    GameState* parent;
    
public:   
    GameState() : parent(NULL) { }
    GameState(GameState* p) : parent(p) { }
    ~GameState() { }
    
    virtual void Init() { }
    virtual void Tick(float dt) { }
    virtual void Render() { }
};

class Game {
private:
    static GameState* state;
    GameTimer* timer;
    Gfx::Texture* mainTexture;

public:
    Game();
    ~Game();
    
    static void SetState(GameState* s);
    
    void Tick();
    void Render();
};

//Include states here because the game class has actaully been defined now
#include "states/playstate.hpp"
#include "states/pausestate.hpp"

GameState* Game::state = NULL;
Game::Game() {
    Gfx::Initialize();
    Keys::Initialize();
    Mouse::Initialize();
    Tile::Initialize();

    Gfx::SetSize(16.0f, 12.0f);

    timer = new GameTimer();

    mainTexture = Gfx::LoadTexture("res/tilemap.png");
    mainTexture->Use(0);
   
    Game::state = new PlayState(); 
}

Game::~Game() {
    cout << "Destuctor was called" << endl;
    delete timer;
    delete Game::state;
}

void Game::SetState(GameState* s) {
    s->parent = state;
    s->Init();
    state = s;
}

void Game::Tick() {
    float dt = (float) timer->GetDt();

    Game::state->Tick(dt);

    Keys::Tick();
    Mouse::Tick();
}

void Game::Render() {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Game::state->Render();

    Gfx::SetupDraw();
    glDrawElements(GL_TRIANGLES, MAX_QUADS * 6, GL_UNSIGNED_SHORT, (void*) 0);
    Gfx::CleanupDraw();
}

#endif

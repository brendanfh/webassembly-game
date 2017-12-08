#ifndef __GAME_H_
#define __GAME_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>
#include <stack>

using namespace std;

#include "gfx.hpp"
#include "input.hpp"
#include "world/entities/player.hpp"
#include "world/entities/enemy.hpp"
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

class GameState {
public:   
    GameState() { }
    ~GameState() { }
    
    virtual void Init() { }
    virtual void Deinit() { }
    virtual void Tick(float dt) { }
    virtual void Render() { }
};

class Game {
private:
    static stack<GameState*> states;
    GameTimer* timer;
    Gfx::Texture* mainTexture;

public:
    Game();
    ~Game();
    
    //static void SetState(GameState* s);
    static void AddState(GameState* s);
    static void PopState();
    
    void Tick();
    void Render();
};

//Include states here because the game class has actaully been defined now
#include "states/playstate.hpp"
#include "states/pausestate.hpp"

stack<GameState*> Game::states;
Game::Game() {
    Gfx::Initialize();
    Keys::Initialize();
    Mouse::Initialize();
    Tile::Initialize();

    Gfx::SetSize(16.0f, 9.0f);

    timer = new GameTimer();

    mainTexture = Gfx::LoadTexture("res/tilemap.png");
    mainTexture->Use(0);
   
    Game::AddState(new PlayState());
}

Game::~Game() {
    cout << "Destuctor was called" << endl;
    delete timer;

    while (!states.empty())
        Game::PopState();
}

void Game::AddState(GameState* s) {
    s->Init();
    states.push(s);
}

void Game::PopState() {
    GameState* s = states.top();
    s->Deinit();
    delete s;
    states.pop(); 
}

void Game::Tick() {
    if (states.empty()) return;

    float dt = (float) timer->GetDt();

    Game::states.top()->Tick(dt);

    Keys::Tick();
    Mouse::Tick();
}

void Game::Render() {
    if (states.empty()) return;

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Game::states.top()->Render();

    Gfx::SetupDraw();
    glDrawElements(GL_TRIANGLES, MAX_QUADS * 6, GL_UNSIGNED_SHORT, (void*) 0);
    Gfx::CleanupDraw();
}

#endif

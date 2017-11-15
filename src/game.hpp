#ifndef __GAME_H_
#define __GAME_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

#include "gfx.hpp"
#include "input.hpp"
#include "world/player.hpp"
#include "world/enemy.hpp"
#include "world/world.hpp"
#include "world/tilemap.hpp"

using namespace std;

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

class Game {
private:
    GameTimer* timer;
    World* world;
    Gfx::Texture* mainTexture;

public:
    Game() { 
        Gfx::Initialize();
        Keys::Initialize();
        Mouse::Initialize();
        Tile::Initialize();

        Gfx::SetSize(16.0f, 12.0f);

        timer = new GameTimer();
        world = new World();

        mainTexture = Gfx::LoadTexture("res/tilemap.png");
        mainTexture->Use(0);

        Player* ply = new Player();
        ply->SetRenderOrder(1);
        world->AddEntity(ply);

        for (int i = 0; i < 100; i++) {
            Enemy* enemy = new Enemy(ply);
            enemy->SetRenderOrder(0);
            world->AddEntity(enemy);
        }
    }

    ~Game() {
        cout << "Destuctor was called" << endl;
        delete timer;
        delete world;
    }

    void tick() {
        float dt = (float) timer->GetDt();

        world->Tick(dt);

        Keys::Tick();
        Mouse::Tick();
    }

    void render() {
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world->Render();

        Gfx::SetupDraw();
        glDrawElements(GL_TRIANGLES, MAX_QUADS * 6, GL_UNSIGNED_SHORT, (void*) 0);
        Gfx::CleanupDraw();
    }
};

#endif

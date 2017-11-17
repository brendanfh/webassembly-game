#ifndef __PLAYSTATE_H_
#define __PLAYSTATE_H_

#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <iostream>

#include "../gfx.hpp"
#include "../input.hpp"
#include "../world/player.hpp"
#include "../world/enemy.hpp"
#include "../world/world.hpp"
#include "../world/tilemap.hpp"

#include "../game.hpp"

class PlayState : public GameState {
    World* world;
public:
    PlayState() : GameState(NULL) {
        world = new World();
        
        Player* ply = new Player();
        ply->SetRenderOrder(1);
        world->AddEntity(ply);
    
        for (int i = 0; i < 100; i++) {
            Enemy* enemy = new Enemy(ply);
            enemy->SetRenderOrder(0);
            world->AddEntity(enemy);
        }
    }
    
    ~PlayState() {
        delete world;
    }
    
    void Tick(float dt) override {
        world->Tick(dt); 
        
        if (Keys::IsDown(GLFW_KEY_Z)) {
            Game::SetState(NULL);
        }
    }
    
    void Render() override {
        world->Render();
    }
};

#endif
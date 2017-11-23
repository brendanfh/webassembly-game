#ifndef __PLAYSTATE_H_
#define __PLAYSTATE_H_

#include <cmath>

#include "../gfx.hpp"
#include "../input.hpp"
#include "../world/player.hpp"
#include "../world/enemy.hpp"
#include "../world/world.hpp"
#include "../world/tilemap.hpp"

#include "../utils/font.hpp"

#include "../game.hpp"
#include "pausestate.hpp"

class PlayState : public GameState {
    World* world;
    
    Player* ply;
public:
    PlayState() : GameState(NULL) {
        world = new World();
        
        ply = new Player();
        world->AddEntity(ply);
    }
    
    ~PlayState() {
        delete world;
    }
    
    void Tick(float dt) override {
        world->Tick(dt); 
        if (Keys::IsJustDown(GLFW_KEY_P)) {
            Game::SetState(new PauseState(this));
        }
    }
    
    void Render() override {
        world->Render();
        
        Gfx::Font::Draw("Test1234 Aj", 900, ply->x, ply->y, 0.4f);

        float xx = (ply->x - Gfx::width / 2) / 2.0f;
        float yy = (ply->y - Gfx::height / 2) / 2.0f;
        xx = floor(xx * Gfx::width) / Gfx::width;
        yy = floor(yy * Gfx::height) / Gfx::height;
        Gfx::CenterOn(xx + Gfx::width / 2, yy + Gfx::height / 2);
    }
};

#endif
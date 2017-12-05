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
    PlayState() : GameState() {
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
            Game::AddState(new PauseState());
        }
    }
    
    void Render() override {
        world->Render();
        
        //Player is rounded to the nearest 100th so this makes sure
        //the text is right above the player 
        float x = floor(ply->x * 100) / 100.0f;
        float y = floor(ply->y * 100) / 100.0f;
        Gfx::Font::Draw("Test1234 Aj", 900, x, y, 0.4f);

        float tw = (float) world->GetTilemap()->TotalWidth();
        float th = (float) world->GetTilemap()->TotalHeight();
        float tx = world->GetTilemap()->GetX();
        float ty = world->GetTilemap()->GetY();
        float xx = ((x + tx) - tw / 2) / 2.0f;
        float yy = ((y + ty) - th / 2) / 2.0f;
        Gfx::CenterOn(xx + tw / 2, yy + th / 2);
    }
};

#endif
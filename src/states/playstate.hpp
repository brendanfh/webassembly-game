#ifndef __PLAYSTATE_H_
#define __PLAYSTATE_H_

#include <cmath>

#include "../gfx.hpp"
#include "../input.hpp"
#include "../world/entities/player.hpp"
#include "../world/entities/enemy.hpp"
#include "../world/world.hpp"
#include "../world/tilemap.hpp"

#include "../utils/font.hpp"

#include "../game.hpp"
#include "pausestate.hpp"

class PlayStateGui {

};

class PlayState : public GameState {
    World* world;
    Player* ply;

public:
    PlayState() : GameState() {
        world = loadWorld("res/map.png");
        
        ply = new Player();
        world->AddEntity(ply);

        world->AddEntity(new Enemy(ply));
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
        float px = ply->x;//floor(ply->x * 100.0f) / 100.0f;
        float py = ply->y;//floor(ply->y * 100.0f) / 100.0f;
        Gfx::CenterOn(px + 0.5f, py + 0.5f);
        
        world->Render();
        Gfx::Font::Draw("Test1234 Aj", 1000, 0, 0, 0.4f);
    }
};

#endif
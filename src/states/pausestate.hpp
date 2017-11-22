#ifndef __PAUSESTATE_H_
#define __PAUSESTATE_H_

#include "../gfx.hpp"
#include "../game.hpp"

class PauseState : public GameState {
public:
    PauseState(GameState* p) : GameState(p) {
    }
    
    ~PauseState() {
        GameState::~GameState();
        Gfx::ClearData(2000, 1000); 
    }
    
    void Init() override { }
    void Tick(float dt) override {
        if (Keys::IsJustDown(GLFW_KEY_P)) {
            Game::SetState(parent);
            
            delete this;
        }
    }
    
    void Render() override {
        parent->Render(); 
        
        Gfx::DrawRect(2000, 0, 0, Gfx::width, Gfx::height, 0, 0, 0, 0.7f);
        
        float w = Gfx::Font::GetWidth("Paused", 1.0f);
        Gfx::Font::Draw("Paused", 2001, (Gfx::width - w) / 2, (Gfx::height - 1.0f) / 2, 1.0f);
    }
};

#endif
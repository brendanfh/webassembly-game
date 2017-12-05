#ifndef __PAUSESTATE_H_
#define __PAUSESTATE_H_

#include "../gfx.hpp"
#include "../game.hpp"

class PauseState : public GameState {
public:
    PauseState() : GameState() {
    }
    
    void Init() override { }
    void Deinit() override {
        cout << "Removing gfx data" << endl;
        Gfx::ClearData(2000, 1000);
    }

    void Tick(float dt) override {
        if (Keys::IsJustDown(GLFW_KEY_P)) {
            Game::PopState();
        }
    }
    
    void Render() override {
        //parent->Render(); 
        Gfx::SetOffset(0, 0);
        
        Gfx::DrawRect(2000, 0, 0, Gfx::width, Gfx::height, 0, 0, 0, 0.7f);

        float w = Gfx::Font::GetWidth("Paused", 1.0f);
        Gfx::Font::Draw("Paused", 2001, (Gfx::width - w) / 2, (Gfx::height - 1.0f) / 2, 1.0f);
    }
};

#endif
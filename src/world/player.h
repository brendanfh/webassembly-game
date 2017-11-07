#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "../input.h"
#include "../gfx.h"
#include "entity.h"

#include <iostream>

using namespace std;

class Player : public Entity {
private:
    float x;
    float y;
    Gfx::Texture* tex;

public:
    Player(int id);
    ~Player();
    void Tick(float dt) override;
    void Render() override;
};

#endif
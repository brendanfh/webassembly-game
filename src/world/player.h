#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "../input.h"
#include "../gfx.h"
#include "world.h"
#include "bullet.h"

#include <iostream>

using namespace std;

class Player : public Entity {
private:
    float x;
    float y;

public:
    Player(int id);
    ~Player();

    void Fire();
    void Tick(float dt) override;
    void Render() override;
};

#endif
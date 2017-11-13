#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "../input.h"
#include "../gfx.h"
#include "../utils/animator.h"
#include "world.h"
#include "bullet.h"

#include <cmath>
#include <iostream>

using namespace std;

class Player : public Entity {
private:
    float x;
    float y;

    Animator* anim;

public:
    Player(int id);
    ~Player();

    void Fire();
    void Tick(float dt) override;
    void Render() override;
};

#endif
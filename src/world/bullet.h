#ifndef __BULLET_H_
#define __BULLET_H_

#include "world.h"

#include <iostream>

using namespace std;

class Bullet : public Entity {
private:
    float x;
    float y;
    float dx;
    float dy;

    float life;

    Entity* owner;

public:
    Bullet(int id, float x, float y, float dx, float dy, Entity* owner);
    ~Bullet();

    void Tick(float dt) override;
    void Render() override;
};


#endif
#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "../gfx.h"
#include <iostream>

using namespace std;

class Entity {
protected:
    Gfx::Quad* quad;

public:
    Entity(int id);
    ~Entity();
    virtual void Tick(float dt);
    virtual void Render();
};

#endif
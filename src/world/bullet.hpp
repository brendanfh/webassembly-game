#ifndef __BULLET_H_
#define __BULLET_H_

#include "world.hpp"

#include <iostream>

using namespace std;

class Bullet : public Entity {
private:
    float dx;
    float dy;

    float life;

    Entity* owner;

public:
    Bullet(float x, float y, float dx, float dy, Entity* owner) : Entity() {
        type = EntityType_Bullet;

        this->x = x;
        this->y = y;
        this->dx = dx;
        this->dy = dy;
        this->owner = owner;

        life = 2.0f;
        quad->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
        quad->SetSubTexture(31 * 16, 31 * 16, 16, 16, 512, 512);

        quad->SetRect(x, y, 0.25f, 0.25f);
        UpdateCollRect();
    }

    ~Bullet() {
        Entity::~Entity();
    }

    void Tick(float dt) override {
        Move(dx * dt, dy * dt, 10);
        UpdateDrawRects();

        life -= dt;

        if (life < 0) {
            world->RemoveEntity(this);
        }
    }

    void Render() override {
        quad->BufferData();
    }
};


#endif
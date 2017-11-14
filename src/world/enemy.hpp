#ifndef __ENEMY_H_
#define __ENEMY_H_
#define PI 3.14159265

#include "world.hpp"

#include <cmath>
#include <iostream>

using namespace std;

class Enemy : public Entity {
private:
    Animator* anim;
    Entity* target;

public:
    Enemy(Entity* target) : Entity() {
        quad ->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        this->target = target;

        x = 0.0f;
        y = 0.0f;

        quad->SetRect(x, y, 0.5f, 0.5f);

        anim = new Animator(4);
        anim->SetAnimation(0, 0, 2, 16, 16, 512, 512, 2, 0.2f); //Walking Right
        anim->SetAnimation(1, 0, 3, 16, 16, 512, 512, 2, 0.2f); //Walking Left
        anim->SetAnimation(2, 2, 2, 16, 16, 512, 512, 2, 0.2f); //Walking Down
        anim->SetAnimation(3, 2, 3, 16, 16, 512, 512, 2, 0.2f); //Walking Up
        anim->ApplyToQuad(quad);
    }

    ~Enemy() {
        Entity::~Entity();
        delete anim;
    }

    void Tick(float dt) override {
        Entity::Tick(dt);

        float tx = target->x - x;
        float ty = target->y - y;

        if (tx * tx + ty * ty < 0.25f) {
            //Close enough to target
        } else {
            float a = atan2(ty, tx);
            float dx = cos(a) * 2.0f;
            float dy = sin(a) * 2.0f;

            if (a > -PI / 4 && a < PI / 4) {
                anim->SetActive(0);
            }
            if (a < -PI / 4 && a > -3 * PI / 4) {
                anim->SetActive(3);
            }
            if (abs(a) > 3 * PI / 4) {
                anim->SetActive(1);
            }
            if (a > PI / 4 && a < 3 * PI / 4) {
                anim->SetActive(2);
            }

            x += dx * dt;
            y += dy * dt;
            anim->Tick(dt);
        }
    }

    void Render() override {
        quad->SetRect(x, y, 1.0f, 1.0f);
        anim->ApplyToQuad(quad);
        quad->BufferData(); 
    }
};


#endif
#ifndef __ENEMY_H_
#define __ENEMY_H_
#define PI 3.14159265

#include "../world.hpp"
#include "particles.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

class Enemy : public Entity {
private:
    Animator* anim;
    Entity* target;

    Gfx::Quad* healthBar;

protected:
    void UpdateCollRect() override {
        collRect->Set(x + 3.0f/16.0f, y + 9.0f/16.0f, 11.0f/16.0f, 7.0f/16.0f);
    }

public:
    Enemy(Entity* target) : Entity() {
        type = EntityType::Enemy;
        quad->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        
        this->target = target;
        health = 10;

        healthBar = new Gfx::Quad(-1);
        healthBar->SetSubTexture(31.0f, 31.0f, 1.0f, 1.0f, 32.0f, 32.0f);
        healthBar->SetColor(1.0f, 0.0f, 0.0f, 1.0f);

        x = 1.0f * (rand() % 11) + 4.0f;
        y = 1.0f * (rand() % 11);

        quad->SetRect(x, y, 1.0f, 1.0f);

        anim = new Animator(4);
        anim->SetAnimation(0, 0, 2, 16, 16, 512, 512, 2, 0.2f); //Walking Right
        anim->SetAnimation(1, 0, 3, 16, 16, 512, 512, 2, 0.2f); //Walking Left
        anim->SetAnimation(2, 2, 2, 16, 16, 512, 512, 2, 0.2f); //Walking Down
        anim->SetAnimation(3, 2, 3, 16, 16, 512, 512, 2, 0.2f); //Walking Up
        anim->ApplyToQuad(quad);

        UpdateCollRect();
    }

    ~Enemy() {
        Entity::~Entity();
        delete anim;
        delete healthBar;
    }

    void Hurt(int damage) override {
        Entity::Hurt(damage);
        
        if (!alive) {
            //Spawn gore
            for (int i = 0; i < 20; i++) {
                GoreParticle* gore = new GoreParticle(x, y);
                world->AddEntity(gore);
            }
        } 
    }

    void Tick(float dt) override {
        Entity::Tick(dt);

        float tx = target->x - x;
        float ty = target->y - y;

        if (tx * tx + ty * ty < 1.0f) {
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

            Move(dx * dt, dy * dt, 10);
            UpdateDrawRects();
            anim->Tick(dt);
        }
    }

    void Render() override {
        anim->ApplyToQuad(quad);
        quad->BufferData(); 

        healthBar->SetRect(x, y - 0.25f, 1.0f * (health / 10.f), 0.25f);
        healthBar->id = 4000;
        healthBar->BufferData();
    }
};


#endif
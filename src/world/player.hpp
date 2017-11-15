#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "../input.hpp"
#include "../gfx.hpp"
#include "../utils/animator.hpp"
#include "world.hpp"
#include "bullet.hpp"

#include <cmath>
#include <iostream>

using namespace std;

class Player : public Entity {
private:
    Animator* anim;

protected:
    void UpdateCollRect() override {
        collRect->Set(x + 3.0f/16.0f, y + 9.0f/16.0f, 11.0f/16.0f, 7.0f/16.0f);
    }

public:
    Player() : Entity() {
        type = EntityType_Player;
        quad->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

        x = 0.0f;
        y = 0.0f;

        quad->SetRect(x, y, 1.0f, 1.0f);

        float walkingAnimSpeed = 0.2f;

        anim = new Animator(6);
        anim->SetAnimation(0, 1, 0, 16, 16, 512, 512, 1, 1.0f); // Standing-still right
        anim->SetAnimation(1, 1, 1, 16, 16, 512, 512, 1, 1.0f); // Standing-still left
        anim->SetAnimation(2, 0, 0, 16, 16, 512, 512, 4, walkingAnimSpeed); // Walking right
        anim->SetAnimation(3, 0, 1, 16, 16, 512, 512, 4, walkingAnimSpeed); // Walking left
        anim->SetAnimation(4, 4, 0, 16, 16, 512, 512, 2, walkingAnimSpeed); // Walking down
        anim->SetAnimation(5, 4, 1, 16, 16, 512, 512, 2, walkingAnimSpeed); // Walking up
        anim->ApplyToQuad(quad);

        UpdateCollRect();
    }

    ~Player() {
        Entity::~Entity();
        delete anim;
    }

    void Fire() { 
        float mx, my;
        Mouse::GetScreenPos(mx, my);

        float bulletSpeed = 10.0f;

        float fx = x + 0.5f;
        float fy = y + 0.5f;

        float a = atan2(my - fy, mx - fx);
        float dx = cos(a) * bulletSpeed;
        float dy = sin(a) * bulletSpeed;

        Bullet* bullet = new Bullet(fx + dx * 0.1f, fy + dy * 0.1f, dx, dy, this);
        this->world->AddEntity(bullet);
    }

    void Tick(float dt) override { 
        Entity::Tick(dt);

        if (Keys::IsJustDown(GLFW_KEY_SPACE)) {
            this->Fire();
        }

        float dx = 0;
        float dy = 0;

        if (Keys::IsDown(GLFW_KEY_A)) {
            dx += -4.0f * dt;
            anim->SetActive(3);
        }
        if (Keys::IsDown(GLFW_KEY_D)) {
            dx += 4.0f * dt;
            anim->SetActive(2);
        }
        if (Keys::IsDown(GLFW_KEY_W)) {
            dy += -4.0f * dt;
            anim->SetActive(5);
        }
        if (Keys::IsDown(GLFW_KEY_S)) {
            dy += 4.0f * dt;
            anim->SetActive(4);
        }

        if (dx != 0 || dy != 0) {
            Move(dx, dy, 10);

            anim->Tick(dt);
            UpdateDrawRects();
        }
    }
    void Render() override { 
        anim->ApplyToQuad(quad);
        quad->BufferData(); 
    }
};

#endif
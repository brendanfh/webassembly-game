#include "player.h"

Player::Player(int id) : Entity(id) {
    quad->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    x = 0.0f;
    y = 0.0f;

    quad->SetRect(x, y, 2.0f, 2.0f);

    anim = new Animator(3);
    anim->SetAnimation(0, 0, 0, 16, 16, 512, 512, 2, 0.5f);
    anim->SetAnimation(1, 1, 0, 16, 16, 512, 512, 1, 1.0f);
    anim->ApplyToQuad(quad);
}

Player::~Player() {
    Entity::~Entity();
    delete anim;
}

void Player::Fire() {
    float mx, my;
    Mouse::GetScreenPos(mx, my);

    float bulletSpeed = 10.0f;

    float a = atan2(my - y, mx - x);
    float dx = cos(a) * bulletSpeed;
    float dy = sin(a) * bulletSpeed;

    Bullet* bullet = new Bullet(-1, x, y, dx, dy, this);
    this->world->AddEntity(bullet);
}

void Player::Tick(float dt) {
    Entity::Tick(dt);

    if (Keys::IsJustDown(GLFW_KEY_SPACE)) {
        this->Fire();
    }

    if (Keys::IsDown(GLFW_KEY_W)) {
        y -= 4.0f * dt;
    }
    if (Keys::IsDown(GLFW_KEY_S)) {
        y += 4.0f * dt;
    }
    if (Keys::IsDown(GLFW_KEY_A)) {
        x -= 4.0f * dt;
    }
    if (Keys::IsDown(GLFW_KEY_D)) {
        x += 4.0f * dt;
    }

    if (Keys::IsDown(GLFW_KEY_P)) {
        anim->SetActive(1);
    }

    anim->Tick(dt);
}

void Player::Render() {
    quad->SetRect(x, y, 4.0f, 4.0f);
    anim->ApplyToQuad(quad);
    quad->BufferData();
}
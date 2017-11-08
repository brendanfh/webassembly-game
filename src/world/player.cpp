#include "player.h"

Player::Player(int id) : Entity(id) {
    quad->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    x = 0.0f;
    y = 0.0f;

    quad->SetRect(x, y, 2.0f, 2.0f);
    quad->SetSubTexture(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

Player::~Player() {
    Entity::~Entity();
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
}

void Player::Render() {
    quad->SetRect(x, y, 2.0f, 2.0f);
    quad->BufferData();
}
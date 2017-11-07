#include "bullet.h"

Bullet::Bullet(int id, float x, float y, float dx, float dy, Entity* owner) : Entity(id) {
    this->x = x;
    this->y = y;
    this->dx = dx;
    this->dy = dy;
    this->owner = owner;

    life = 2.0f;
    quad->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    quad->SetSubTexture(0.0f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f);
}

Bullet::~Bullet() {
    Entity::~Entity();
}

void Bullet::Tick(float dt) {
    x += dx * dt;
    y += dy * dt;
    life -= dt;

    if (life < 0) {
        cout << "SHOULD DIEEEEEE!!!!" << endl;

        quad->SetRect(0.0f, 0.0f, 0.0f, 0.0f);
        quad->BufferData();
        world->RemoveEntity(this);
    }
}

void Bullet::Render() {
    quad->SetRect(x, y, 0.25f, 0.25f);
    quad->BufferData();
}
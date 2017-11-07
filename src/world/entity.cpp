#include "entity.h"

Entity::Entity(int id) {
    quad = new Gfx::Quad(id);
}

Entity::~Entity() {
    delete quad;
}

void Entity::Tick(float dt) {
}

void Entity::Render() {
}
#include "world.h"

Entity::Entity(int renderOrder) {
    quad = new Gfx::Quad(renderOrder);
}

Entity::~Entity() {
    delete quad;
}

void Entity::SetRenderOrder(int renderOrder) {
    quad->id = renderOrder;
}

void Entity::Tick(float dt) {
}

void Entity::Render() {
}

World::World() {
    nextId = 0;

    entities = new vector<Entity*>();
}

World::~World() {
    vector<Entity*>::iterator it;
    for (it = entities->begin(); it != entities->end(); it++) {
        delete *it;
    }
    delete entities;
}

void World::AddEntity(Entity* entity) {
    entity->world = this;
    entity->SetRenderOrder(nextId++);
    entities->push_back(entity);
}

void World::RemoveEntity(Entity* entity) {
    int i;
    for (i = 0; i < entities->size(); i++) {
        if ((*entities)[i] == entity) {
            break;
        }
    }

    delete *(entities->begin() + i);
    entities->erase(entities->begin() + i);
}

void World::Tick(float dt) {
    for (int i = 0; i < entities->size(); i++) {
        (*entities)[i]->Tick(dt);
    }
}

void World::Render() {
    for (int i = 0; i < entities->size(); i++) {
        (*entities)[i]->Render();
    }
}
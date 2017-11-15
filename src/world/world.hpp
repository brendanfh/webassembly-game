#ifndef __WORLD_H_
#define __WORLD_H_

#include "../gfx.hpp"
#include "../utils/rect.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

class World;
class Entity;

using namespace std;

enum EntityType {
    EntityType_Player = 0,
    EntityType_Enemy = 1,
    EntityType_Bullet = 2
};

class Entity {
    friend World;
protected:
    EntityType type;
    Gfx::Quad* quad;
    Rect* drawRect;
    Rect* collRect;

    virtual void UpdateCollRect();

public:
    float x;
    float y;
    World* world;

    Entity();
    ~Entity();

    void SetRenderOrder(int renderOrder);

    Rect* GetCollisionRect();
    EntityType GetType();

    void UpdateDrawRects();

    void Move(float dx, float dy, int steps);

    virtual void Tick(float dt) { }
    virtual void Render() { }
};

class World {
private:
    vector<Entity*>* entities;

public:
    World() {
        entities = new vector<Entity*>();
    }

    ~World() {
        vector<Entity*>::iterator it;
        for (it = entities->begin(); it != entities->end(); it++) {
            delete *it;
        }
        delete entities;
    }

    void AddEntity(Entity* entity) {
        entity->world = this;
        entities->push_back(entity);
    }

    void RemoveEntity(Entity* entity) {
        int i;
        for (i = 0; i < entities->size(); i++) {
            if ((*entities)[i] == entity) {
                break;
            }
        }

        delete *(entities->begin() + i);
        entities->erase(entities->begin() + i);
    }

    vector<Rect*>* RectsInRange(float x, float y, float r) {
        float r2 = r * r;
        vector<Rect*>* rects = new vector<Rect*>();

        std::for_each(entities->begin(), entities->end(), [=](Entity*& e) {
            float dx = (e->x - x);
            float dy = (e->y - y);

            if (dx * dx + dy * dy < r2) {
                rects->push_back(e->collRect);
            }
        });

        return rects;
    }

    void Tick(float dt) { 
        for (int i = 0; i < entities->size(); i++) {
            (*entities)[i]->Tick(dt);
        }
    }

    void Render() {
        std::sort(entities->begin(), entities->end(), [](Entity* a, Entity* b) {
            return a->collRect->y < b->collRect->y;
        });

        for (int i = 0; i < entities->size(); i++) {
            (*entities)[i]->SetRenderOrder(i);
            (*entities)[i]->Render();
        }
    }
};

void Entity::UpdateCollRect() {
    collRect->Set(x, y, drawRect->w, drawRect->h);
}

Entity::Entity() {
    quad = new Gfx::Quad(-1);

    drawRect = new Rect(0.0f, 0.0f, 0.0f, 0.0f);
    collRect = new Rect(0.0f, 0.0f, 0.0f, 0.0f);
}

Entity::~Entity() {
    quad->SetRect(0, 0, 0, 0);
    quad->BufferData();
    delete quad;

    delete drawRect;
    delete collRect;
}

void Entity::SetRenderOrder(int renderOrder) {
    if (renderOrder < quad->id) {
        quad->SetRect(0, 0, 0, 0);
        quad->BufferData();
        UpdateDrawRects();
    }
    quad->id = renderOrder;
}

Rect* Entity::GetCollisionRect() {
    return collRect;
}

EntityType Entity::GetType() {
    return type;
}

void Entity::UpdateDrawRects() {
    drawRect->Set(x, y, drawRect->w, drawRect->h);
    quad->SetRect(x, y, drawRect->w, drawRect->h);
}

void Entity::Move(float dx, float dy, int steps) {
    float ddx = dx / steps;
    float ddy = dy / steps;

    vector<Rect*>* ents = world->RectsInRange(x + 0.5f, y + 0.5f, 2.0f + (dx * dx + dy * dy));

    bool xdone = false;
    bool ydone = false;
    bool collided = false;
    for (int i = 0; i < steps; i++) {
        if (!xdone && ddx != 0) {
            x += ddx;
            this->UpdateCollRect();
            collided = std::any_of(ents->begin(), ents->end(), [=](Rect* r) {
                if (r == this->collRect) return false;
                return r->Intersects(*this->collRect);
            });

            if (collided) {
                x -= ddx;
                this->UpdateCollRect();
                xdone = true;
            }
        }
        if (!ydone && ddy != 0) {
            y += ddy;
            this->UpdateCollRect();
            collided = std::any_of(ents->begin(), ents->end(), [=](Rect* r) {
                if (r == this->collRect) return false;
                return r->Intersects(*this->collRect);
            });

            if (collided) {
                y -= ddy;
                this->UpdateCollRect();
                ydone = true;
            }
        }
    }
    this->UpdateCollRect();
    
    delete ents;
}

#endif
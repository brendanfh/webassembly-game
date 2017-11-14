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

    vector<Entity*>* EntitiesInRange(float x, float y, float r) {
        float r2 = r * r;
        vector<Entity*>* ents = new vector<Entity*>();

        std::for_each(entities->begin(), entities->end(), [=](Entity*& e) {
            float dx = (e->x - x);
            float dy = (e->y - y);

            if (dx * dx + dy * dy < r2) {
                ents->push_back(e);
            }
        });

        return ents;
    }

    void Tick(float dt) { 
        for (int i = 0; i < entities->size(); i++) {
            (*entities)[i]->Tick(dt);
        }
    }

    void Render() {
        std::sort(entities->begin(), entities->end(), [](Entity* a, Entity* b) {
            return a->y < b->y;
        });

        for (int i = 0; i < entities->size(); i++) {
            (*entities)[i]->SetRenderOrder(i);
            (*entities)[i]->Render();
        }
    }
};


#endif
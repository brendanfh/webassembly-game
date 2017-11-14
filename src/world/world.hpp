#ifndef __WORLD_H_
#define __WORLD_H_

#include "../gfx.hpp"
#include <vector>
#include <algorithm>

class World;
class Entity;

using namespace std;

class Entity {
protected:
    Gfx::Quad* quad;

public:
    float x;
    float y;
    World* world;

    Entity() {
        quad = new Gfx::Quad(-1);
    }

    ~Entity() {
        quad->SetRect(0, 0, 0, 0);
        quad->BufferData();
        delete quad;
    }

    void SetRenderOrder(int renderOrder) {
        if (renderOrder < quad->id) {
            quad->SetRect(0, 0, 0, 0);
            quad->BufferData();
        }
        quad->id = renderOrder;
    }

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
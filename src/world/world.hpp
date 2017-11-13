#ifndef __WORLD_H_
#define __WORLD_H_

#include "../gfx.hpp"
#include <vector>

class World;
class Entity;

using namespace std;

class Entity {
protected:
    Gfx::Quad* quad;

public:
    World* world;

    Entity(int renderOrder) {
        quad = new Gfx::Quad(renderOrder);
    }

    ~Entity() {
        delete quad;
    }

    void SetRenderOrder(int renderOrder) {
        quad->id = renderOrder;
    }

    virtual void Tick(float dt) { }
    virtual void Render() { }
};

class World {
private:
    int nextId;

    vector<Entity*>* entities;

public:
    World() {
        nextId = 0;

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
        entity->SetRenderOrder(nextId++);
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
        for (int i = 0; i < entities->size(); i++) {
            (*entities)[i]->Render();
        }
    }
};


#endif
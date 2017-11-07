#ifndef __WORLD_H_
#define __WORLD_H_

#include "../gfx.h"
#include <vector>

class World;
class Entity;

using namespace std;

class Entity {
protected:
    Gfx::Quad* quad;

public:
    World* world;

    Entity(int renderOrder);
    ~Entity();

    void SetRenderOrder(int renderOrder);
    virtual void Tick(float dt);
    virtual void Render();
};

class World {
private:
    int nextId;

    vector<Entity*>* entities;

public:
    World();
    ~World();

    void AddEntity(Entity* entity);
    void RemoveEntity(Entity* entity);

    void Tick(float dt);
    void Render();
};


#endif
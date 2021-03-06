#ifndef __WORLD_H_
#define __WORLD_H_

#include "../gfx.hpp"
#include "../utils/rect.hpp"
#include "tilemap.hpp"
#include "../utils/idmanager.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

class World;
class Entity;

using namespace std;

enum class EntityType {
    Unknown,
    Player,
    Enemy,
    Bullet,
    Particle
};

class Entity {
    friend World;
private:
    virtual bool Move2(vector<Entity*>* ents, float dx, float dy) final;

protected:
    EntityType type;
    Gfx::Quad* quad;
    Rect* collRect;

    int health;
    bool alive;
    bool solid;

    virtual void UpdateCollRect() {
        collRect->Set(x, y, collRect->w, collRect->h);
    }

public:
    float x;
    float y;
    World* world;

    Entity() {
        type = EntityType::Unknown;
        quad = new Gfx::Quad(-1);

        health = 0;
        solid = true;
        alive = true;

        collRect = new Rect(0.0f, 0.0f, 0.0f, 0.0f);
    }

    ~Entity() {
        delete quad;
        delete collRect;
    }

    void SetRenderOrder(int renderOrder) {
        if (renderOrder < quad->id) {
            float x, y, w, h;
            quad->GetRect(x, y, w, h);
            quad->SetRect(0, 0, 0, 0);
            quad->BufferData();
            quad->SetSize(w, h);
            UpdateDrawRects();
        }
        quad->id = renderOrder;
    }

    Rect* GetCollisionRect() {
        return collRect;
    }

    EntityType GetType() {
        return type;
    }

    bool IsAlive() {
        return alive;
    }

    virtual void UpdateDrawRects() {
        quad->SetPos(x, y);
    }

    void Move(float dx, float dy, int steps);

    virtual void OnTileCollision(int tx, int ty, float dx, float dy) {
        x -= dx;
        y -= dy;
    }

    virtual void OnEntityCollision(Entity* other, float dx, float dy) {
        x -= dx;
        y -= dy;
    }

    virtual void Hurt(int damage) {
        health -= damage; 

        if (health <= 0)
            alive = false;
    }

    virtual void Tick(float dt) { }
    virtual void Render() { }
};

class World {
private:
    vector<Entity*> entities;
    Tilemap* tilemap;

public:
    friend World* loadWorld(string path);
    Entity* player;

    World() {
        entities = vector<Entity*>();
        tilemap = new Tilemap(20, 20);
    }

    ~World() {
        vector<Entity*>::iterator it;
        for (it = entities.begin(); it != entities.end(); it++) {
            delete *it;
        }
        //delete entities;
    }

    Tilemap* GetTilemap() {
        return tilemap;
    }

    void AddEntity(Entity* entity) {
        entity->world = this;
        if (entity->type == EntityType::Player) {
            player = entity;
        }
        entities.push_back(entity);
    }

    void RemoveEntity(Entity* entity) {
        int i;
        for (i = 0; i < entities.size(); i++) {
            if (entities[i] == entity) {
                break;
            }
        }

        delete *(entities.begin() + i);
        entities.erase(entities.begin() + i);
    }

    vector<Entity*>* EntsInRange(float x, float y, float r) {
        float r2 = r * r;
        vector<Entity*>* ents = new vector<Entity*>();

        std::for_each(entities.begin(), entities.end(), [=](Entity*& e) {
            if (!e->solid) return;

            float dx = (e->x - x);
            float dy = (e->y - y);

            if (dx * dx + dy * dy < r2) {
                ents->push_back(e);
            }
        });

        return ents;
    }

    void Tick(float dt) { 
        for (int i = 0; i < entities.size(); i++) {
            entities[i]->Tick(dt);

            if (!entities[i]->IsAlive()) {
                RemoveEntity(entities[i]);
            }
        }
    }

    void Render() {
        std::sort(entities.begin(), entities.end(), [](Entity* a, Entity* b) {
            return a->collRect->y < b->collRect->y;
        });

        float px, py;
        std::any_of(entities.begin(), entities.end(), [&](Entity* a) {
            if (a->GetType() == EntityType::Player) {
                px = a->x;
                py = a->y;
                return true;
            }

            return false;
        });

        //Tilemap::Render returns how many ids it used
        IdMng::Use(0);
        tilemap->Render(px, py);

        for (int i = 0; i < entities.size(); i++) {
            entities[i]->SetRenderOrder(IdMng::Next());
            entities[i]->Render();
        }
    }
};

//This has to be at the bottom because it references "RectsInRange", which has not been defined at the top yet.
void Entity::Move(float dx, float dy, int steps) {
    float ddx = dx / steps;
    float ddy = dy / steps;

    vector<Entity*>* ents = world->EntsInRange(x + 0.5f, y + 0.5f, 2.0f + (dx * dx + dy * dy));

    Entity* tmp;
    int tx, ty;

    bool xdone = false;
    bool ydone = false;
    for (int i = 0; i < steps; i++) {
        if (!xdone && ddx != 0) {
            xdone = Move2(ents, ddx, 0);
        }
        if (!ydone && ddy != 0) {
            ydone = Move2(ents, 0, ddy);
        }
    }
    this->UpdateCollRect();
    
    delete ents;
}

bool Entity::Move2(vector<Entity*>* ents, float dx, float dy) {
    x += dx;
    y += dy;

    Entity* tmp;
    int tx, ty;

    this->UpdateCollRect();

    bool collided = false;
    collided = any_of(ents->begin(), ents->end(), [&](Entity* e) mutable {
        if (e == this) return false;
        tmp = e;
        return e->collRect->Intersects(*this->collRect);
    });

    if (collided) {
        OnEntityCollision(tmp, dx, dy);
        this->UpdateCollRect();
        return true;
    } else {
        int xx = floor(x / TILE_SIZE);
        int yy = floor(y / TILE_SIZE);

        for (int yt = yy - 2; yt <= yy + 2; yt++) {
            for (int xt = xx - 2; xt <= xx + 2; xt++) {
                Tile* tile = world->GetTilemap()->GetTile(xt, yt);
                
                if (tile == NULL) continue;
                if (!tile->IsSolid()) continue;

                if (tile->GetRect(xt, yt).Intersects(*collRect)) {
                    OnTileCollision(xt, yt, dx, dy);
                    return true; 
                }
            }
        }
    }

    return false;
}

#endif
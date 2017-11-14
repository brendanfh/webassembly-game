#include "world.hpp"

//private function
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

    vector<Entity*>* ents = world->EntitiesInRange(x + 0.5f, y + 0.5f, 2.0f + (dx * dx + dy * dy));

    bool xdone = false;
    bool ydone = false;
    bool collided = false;
    for (int i = 0; i < steps; i++) {
        if (!xdone && ddx != 0) {
            x += ddx;
            this->UpdateCollRect();
            collided = std::any_of(ents->begin(), ents->end(), [=](Entity* e) {
                if (e == this) return false;
                return e->collRect->Intersects(*this->collRect);
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
            collided = std::any_of(ents->begin(), ents->end(), [=](Entity* e) {
                if (e == this) return false;
                return e->collRect->Intersects(*this->collRect);
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
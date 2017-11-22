#ifndef __TILEMAP_H_
#define __TILEMAP_H_

#include "../gfx.hpp"

#include <vector>

#define TILE_SIZE 1.0f

class Tile;
class GrassTile;
class StoneTile;
class Tilemap;

vector<Tile*>* Tiles;

class Tile {
    friend Tilemap;
protected:
    int id;
    bool solid;
    Gfx::Quad* quad;

public:
    static void Initialize();
    static void Uninitialize(); 

    Tile(int id) {
        this->id = id;
        this->solid = false;

        quad = new Gfx::Quad(-1);

        (*Tiles)[id] = this;
    }

    ~Tile() {
        delete quad;
    }

    virtual Rect* GetRect(int x, int y) {
        return new Rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }

    virtual void Render(int id, int x, int y) {
        quad->id = id;
        quad->SetRect(x, y, TILE_SIZE, TILE_SIZE);
        quad->BufferData();
    }
};

class GrassTile : public Tile {
public:
    static const int id = 0;
    
    GrassTile() : Tile(GrassTile::id) {
        quad->SetColor(0.1f, 0.9f, 0.2f, 1.0f);
        quad->SetSubTexture(31.0f, 31.0f, 1.0f, 1.0f, 32.0f, 32.0f);
    }
};

class StoneTile : public Tile {
public:
    static const int id = 1;

    StoneTile() : Tile(StoneTile::id) {
        this->solid = true;
        quad->SetColor(0.4f, 0.4f, 0.4f, 1.0f);
        quad->SetSubTexture(31.0f, 31.0f, 1.0f, 1.0f, 32.0f, 32.0f);
    }
};

void Tile::Initialize() {
    Tiles = new vector<Tile*>(100);

    new GrassTile();
    new StoneTile();
}

void Tile::Uninitialize() {
    for (int i = 0; i < Tiles->size(); i++) {
        delete (*Tiles)[i];
    }
    delete Tiles;
}

class Tilemap {
private:
    int w;
    int h;
    int* tiles;

    Rect** rects;

public:
    Tilemap(int w, int h) : w(w), h(h) {
        tiles = new int[w * h];
        for (int i = 0; i < w * h; i++) {
            tiles[i] = 0;
        }

        rects = new Rect*[w * h];
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                rects[x + y * w] = GetTile(x, y)->GetRect(x, y);
            }
        }
    }

    ~Tilemap() {
        delete[] tiles;

        for (int i = 0; i < w * h; i++) {
            delete rects[i];
        }
        delete[] rects;
    }

    Tile* GetTile(int x, int y) {
        if (x < 0 || y < 0 || x >= w || y >= h) return NULL;
        return (*Tiles)[tiles[x + y * w]];
    }

    void SetTile(int x, int y, int id) {
        if (x < 0 || y < 0 || x >= w || y >= h) return;
        tiles[x + y * w] = id;

        delete rects[x + y * w];
        rects[x + y * w] = (*Tiles)[id]->GetRect(x, y);
    }

    vector<Rect*>* GetSolidTiles(float x, float y, float r) {
        int ix = (int) x;
        int iy = (int) y;
        int ir = (int) (r * 3 + 1);

        vector<Rect*>* rs = new vector<Rect*>();

        for (int yy = iy - ir; yy <= iy + ir; yy++) {
            for (int xx = ix - ir; xx <= ix + ir; xx++) {
                Tile* t = GetTile(xx, yy);
                if (t != NULL) {
                    if (t->solid) {
                        rs->push_back(rects[xx + yy * w]);
                    }
                }
            }
        } 

        return rs;
    }

    void Render() {
        int did = 0;
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                GetTile(x, y)->Render(did++, x, y);
            }
        }
    }
};

#endif
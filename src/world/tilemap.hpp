#ifndef __TILEMAP_H_
#define __TILEMAP_H_

#include "../gfx.hpp"
#include "../game.hpp"

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

    virtual Rect GetRect(int x, int y) {
        return Rect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
    }

    virtual bool IsSolid() {
        return solid;
    }

    Rect* GetRectPtr(int x, int y) {
        Rect r = this->GetRect(x, y);
        Rect* rptr = new Rect(r.x, r.y, r.w, r.h);
        return rptr;
    }

    virtual void Render(int id, int x, int y) {
        quad->id = id;
        //Still need a better solution because this is still shit but it works for now
        quad->SetRect(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE + 0.01f, TILE_SIZE + 0.01f);
        quad->BufferData();
    }
};

class FloorTile : public Tile {
public:
    static const int id = 0;
    
    FloorTile() : Tile(FloorTile::id) {
        quad->SetColor(0.5f, 0.5f, 0.5f, 1.0f);
        quad->SetSubTexture(0.0f, 9.0f, 1.0f, 1.0f, 32.0f, 32.0f);
    }
};

class WallTile : public Tile {
public:
    static const int id = 1;

    WallTile() : Tile(WallTile::id) {
        this->solid = true;
        quad->SetColor(0.7f, 0.7f, 0.7f, 1.0f);
        quad->SetSubTexture(0.0f, 10.0f, 1.0f, 1.0f, 32.0f, 32.0f);
    }
};

class LavaTile : public Tile {
public:
    static const int id = 2;

    int frame = 0;

    LavaTile() : Tile(LavaTile::id) {
        quad->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
        quad->SetSubTexture(0.0f, 11.0f, 1.0f, 1.0f, 32.0f, 32.0f);
    }

    void Render(int id, int x, int y) override {
        int u = Game::UPDATES;
        u /= 43;
        if (u % 4 != frame) {
            frame = u % 4;
            quad->SetSubTexture((-abs(frame - 2) + 2) * 1.0f, 11.0f, 1.0f, 1.0f, 32.0f, 32.0f);
        }
        Tile::Render(id, x, y);
    }
};

class WaterTile : public Tile {
public:
    static const int id = 3;

    int frame = 0;

    WaterTile() : Tile(WaterTile::id) {
        quad->SetColor(0.0f, 0.9f, 0.7f, 1.0f);
        quad->SetSubTexture(0.0f, 12.0f, 1.0f, 1.0f, 32.0f, 32.0f);
    }

    void Render(int id, int x, int y) override {
        int u = Game::UPDATES;
        u /= 21;
        if (u % 4 != frame) {
            frame = u % 4;
            quad->SetSubTexture((-abs(frame - 2) + 2) * 1.0f, 12.0f, 1.0f, 1.0f, 32.0f, 32.0f);
        }
        Tile::Render(id, x, y);
    }
};

void Tile::Initialize() {
    Tiles = new vector<Tile*>(100);

    new FloorTile();
    new WallTile();
    new LavaTile();
    new WaterTile();
}

void Tile::Uninitialize() {
    for (int i = 0; i < Tiles->size(); i++) {
        delete (*Tiles)[i];
    }
    delete Tiles;
}

class Tilemap {
private:
    int x; //where the tilemap exists in world space
    int y;

    int w;
    int h;
    int* tiles;
    int* data;

public:
    Tilemap(int w, int h) : w(w), h(h) {
        x = 0;
        y = 0;

        tiles = new int[w * h];
        data = new int[w * h];
        for (int i = 0; i < w * h; i++) {
            tiles[i] = 0;
            data[i] = 0;
        }
    }

    ~Tilemap() {
        delete[] tiles;
        delete[] data;
    }

    Tile* GetTile(int x, int y) {
        if (x < 0 || y < 0 || x >= w || y >= h) return NULL;
        return (*Tiles)[tiles[x + y * w]];
    }

    void SetTile(int x, int y, int id) {
        if (x < 0 || y < 0 || x >= w || y >= h) return;
        tiles[x + y * w] = id;
    }

    int GetData(int x, int y) {
        if (x < 0 || y < 0 || x >= w || y >= h) return 0;
        return data[x + y * w];
    }

    void SetData(int x, int y, int d) {
        if (x < 0 || y < 0 || x >= w || y >= h) return;
        data[x + y * w] = d;
    }

    int GetX() {
        return x;
    }

    int GetY() {
        return y;
    }

    int GetW() {
        return w;
    }

    int GetH() {
        return h;
    }

    int TotalWidth() {
        return w * TILE_SIZE;
    }

    int TotalHeight() {
        return h * TILE_SIZE;
    }

    int Render(int startid, float xo, float yo) {
        int did = startid;

        float hgw = Gfx::width / 2;
        float hgh = Gfx::height / 2;

        int tw = (int)ceil(hgw / TILE_SIZE);
        int th = (int)ceil(hgh / TILE_SIZE);

        int xm = (int)(xo / TILE_SIZE);
        int ym = (int)(yo / TILE_SIZE);

        for (int y = ym - th; y <= ym + th + 1; y++) {
            for (int x = xm - tw; x <= xm + tw + 1; x++) {
                Tile* t = GetTile(x, y);
                if (t == NULL) {
                    Gfx::ClearData(did, 1);
                } else {
                    t->Render(did, x + this->x, y + this->y);
                }
                did++;
            }
        }

        return did;
    }
};

#endif
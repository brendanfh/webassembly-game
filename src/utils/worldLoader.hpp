#ifndef __WORLDLOADER_H_
#define __WORLDLOADER_H_

#include "../world/world.hpp"
#include "../world/entities/player.hpp"
#include "../world/entities/enemy.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

World* loadWorld(string path) {
    SDL_Surface* img;
    if (!(img = IMG_Load(path.c_str()))) {
        cout << "Failed to load world " << path << endl;
        return NULL;
    }

    Tilemap* tm = new Tilemap(img->w, img->h);
    World* w = new World();
    Player* player = new Player();
    w->AddEntity(player);

    unsigned int* pxdata = (unsigned int*)img->pixels;

    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            unsigned int col = pxdata[x + y * img->w];
            int r = col >> 16 & 0xff;
            int g = col >> 8 & 0xff;
            int b = col >> 0 & 0xff;
            tm->SetTile(x, y, FloorTile::id);

            if (col == 0xffffffff) {
                tm->SetTile(x, y, WallTile::id);
            }
            else if (col == 0xffff0000) {
                player->x = x * TILE_SIZE;
                player->y = y * TILE_SIZE;
            }
            else if (col == 0xff00ffff) {
            	tm->SetTile(x, y, LavaTile::id);
            }
            else if (col == 0xffffff00) {
            	tm->SetTile(x, y, WaterTile::id);
            }
            else if (col == 0xfffe0000) {
                w->AddEntity(new Enemy(x * TILE_SIZE, y * TILE_SIZE, player));
            }
        }
    }

    w->tilemap = tm;

    return w;
}


#endif
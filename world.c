#include "world.h"

#include<string.h>
#include<stdlib.h>

void world_init(world_t * world) {
    int i;
    world->tiles = malloc(sizeof(tile_t) * world->width * world->height);

    for (i = 0; i < world->width * world->height; i++) {
        int y = i / world->width;
        int x = i % world->width;
        world->tiles[i] = (rand() % 256 - y * 20 > 10) ? TILE_AIR : TILE_SOLID;
    }
}

void world_destroy(world_t * world) {
    free(world->tiles);
}


tile_t world_get_tile(world_t * world, int x, int y) {
    if (x < 0 || y < 0 || x >= world->width || y >= world->height) {
        return TILE_INVALID;
    }

    return world->tiles[x + y * world->width];
}

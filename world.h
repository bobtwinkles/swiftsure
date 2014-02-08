#ifndef _WORLD_H_
#define _WORLD_H_

typedef unsigned char tile_t;

#define TILE_AIR     0
#define TILE_SOLID   1
#define TILE_INVALID 0xFF

#define WORLD_SIZE 120
#define STAGE_WIDTH (WORLD_SIZE / 2)
#define STAGE_HEIGHT (WORLD_SIZE * 7 / 8)

typedef struct world {
    int width, height;
    tile_t * tiles;
} world_t;

void world_init(world_t * world, int seed);
void world_destroy(world_t * world);

tile_t world_get_tile(world_t * world, int x, int y);

#endif

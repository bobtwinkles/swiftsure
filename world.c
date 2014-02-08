#include "world.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "log.h"
#include "sdnoise1234.h"

#define SCALE1 16.f

#define PI 3.14159265
#define E  2.71828182

float normal(float x, float mu, float sigma) {
  // see http://en.wikipedia.org/wiki/Normal_distribution
  float coeficient = 1 / (sigma * sqrt(2 * PI));
  float power = - pow((x - mu), 2) / (2 * sigma * sigma);

  return coeficient * pow(E, power);
}

void world_init(world_t * world) {
  int i;
  world->tiles = malloc(sizeof(tile_t) * world->width * world->height);

  for (i = 0; i < world->width * world->height; i++) {
    int y = i / world->width;
    int x = i % world->width;
    if (x == 0 || x == WORLD_SIZE - 1 || y ==0 || y == WORLD_SIZE - 1) {
      world->tiles[i] = TILE_AIR;
    } else {
      float noise, platform, px, py;
      noise = sdnoise2(x / SCALE1, y / SCALE1, NULL, NULL);

      px = x / (float)WORLD_SIZE;
      py = y / (float)WORLD_SIZE;
      platform  = normal(px - 0.5, 0, 0.2);
      swiftsure_log(9, "%f\n", platform);
      if (platform > 1) {platform = 1;}
      platform += normal(py - 0.5, 0.1, 0.2) - 2.5;
      if (platform > 1) {platform = 1;}
      world->tiles[i] = (platform + noise < 0) ? TILE_AIR : TILE_SOLID;
    }
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

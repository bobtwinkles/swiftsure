#include "game.h"

#include "entity.h"
#include "physics.h"
#include "gamedefs.h"
#include "rendering.h"
#include "input.h"
#include "world.h"

#include <stdlib.h>

entity_t players[MAX_PLAYERS];
phys_object_t *objects[MAX_PLAYERS];

void game_init(void) {
  int i;
  float offset, curr;
  offset = (float)WORLD_SIZE / MAX_PLAYERS;
  curr = offset / 2;
  for (i = 0; i < MAX_PLAYERS; ++i) {
    players[i].rect.x = curr;
    players[i].rect.y = 100;
    players[i].rect.w = 2;
    players[i].rect.h = 3;
    players[i].r = i / (float)MAX_PLAYERS;
    players[i].g = 0.5f + 0.5f * (i / (float) MAX_PLAYERS);
    players[i].b = 1.f - (i / (float)MAX_PLAYERS);
    players[i].air_jumps_max = 2;
    players[i].air_jumps_used = 0;
    objects[i] = physics_add_entity(&players[i]);
    input_set_player(i, objects[i]);

    curr += offset;
  }
}

void game_get_avg_pos(float * x, float * y) {
  int i;
  float x_out, y_out;
  x_out = 0;
  y_out = 0;
  for (i = 0; i < MAX_PLAYERS; ++i) {
    x_out += players[i].rect.x + players[i].rect.w / 2;
    y_out += players[i].rect.y + players[i].rect.h / 2;
  }
  x_out /= MAX_PLAYERS;
  y_out /= MAX_PLAYERS;
  *x = x_out;
  *y = y_out;
}

void game_render_players(void) {
  int i;
  for (i = 0; i < MAX_PLAYERS; ++i) {
    render_entity(&players[i]);
  }
}

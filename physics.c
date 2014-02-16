#include "physics.h"

#include "entity.h"
#include "linkedlist.h"
#include "log.h"
#include "globals.h"
#include "world.h"

#include <stdlib.h>
#include <math.h>

static phys_object_t root = {NULL, NULL, NULL, 0, 0};

#define COLLISION_BL 0x1
#define COLLISION_BR 0x2
#define COLLISION_TL 0x4
#define COLLISION_TR 0x8

int check_collisions(world_t * world, int xmin, int xmax, int ymin, int ymax) {
  int collision_flags = 0;
  if (world_get_tile(world, xmin, ymin) == TILE_SOLID) {
    collision_flags |= COLLISION_BL;
  }
  if (world_get_tile(world, xmin, ymax) == TILE_SOLID) {
    collision_flags |= COLLISION_TL;
  }
  if (world_get_tile(world, xmax, ymin) == TILE_SOLID) {
    collision_flags |= COLLISION_BR;
  }
  if (world_get_tile(world, xmax, ymax) == TILE_SOLID) {
    collision_flags |= COLLISION_TR;
  }
  return collision_flags;
}

static float sign(float x) {
  if (x < 0) {return -1;} else
  if (x > 0) {return  1;} else
             {return  0;}
}

#define SMOOTHING_OFFSET 0.01
#define AIR_RESISTANCE 0.95
#define FRICTION 0.5

void physics_tick(struct world * world, double delta) {
  phys_object_t * curr;
  entity_t * ent;
  int xmin,ymin,xmax,ymax;
  int collision_flags;
  int x, y;
  float dx, dy;

  curr = root.next;
  while (curr) {
    ent = curr->ent;
    //Reset collisions from last frame
    curr->y_hit = 0;
    curr->x_hit = 0;
    //Apply forces
    curr->dy -= 98.1 * delta;
    //Move as far as possible
    dx = curr->dx * delta;
    dy = curr->dy * delta;
    xmin = (int)(ent->x + SMOOTHING_OFFSET);
    xmax = (int)(ent->x + ent->w - SMOOTHING_OFFSET);
    ymin = (int)(ent->y + dy + SMOOTHING_OFFSET);
    ymax = (int)(ent->y + ent->h + dy - SMOOTHING_OFFSET);
    for (x = xmin; x <= xmax; ++x) {
      if (world_get_tile(world, x, ymin) == TILE_SOLID) {
        curr->y_hit = 1;
        curr->last_y_hit_frame = frame;
        ent->air_jumps_used = 0;
        dy = 0;
        ent->y = ymin + 1 + SMOOTHING_OFFSET;
      }
      if (world_get_tile(world, x, ymax) == TILE_SOLID) {
        curr->y_hit = 1;
        curr->last_y_hit_frame = frame;
        ent->air_jumps_used = 0;
        dy = 0;
        ent->y = ymin - SMOOTHING_OFFSET;
      }
    }
    xmin = (int)(ent->x + dx + SMOOTHING_OFFSET);
    xmax = (int)(ent->x + ent->w + dx - SMOOTHING_OFFSET);
    ymin = (int)(ent->y + SMOOTHING_OFFSET);
    ymax = (int)(ent->y + ent->h - SMOOTHING_OFFSET);
    for (y = ymin; y <= ymax; ++y) {
      if (world_get_tile(world, xmin, y) == TILE_SOLID) {
        curr->x_hit = 1;
        curr->last_x_hit_frame = frame;
        dx = 0;
        ent->x = xmin + 1 + SMOOTHING_OFFSET;
      }
      if (world_get_tile(world, xmax, y) == TILE_SOLID) {
        curr->x_hit = 1;
        curr->last_x_hit_frame = frame;
        dx = 0;
        ent->x = xmin - SMOOTHING_OFFSET;
      }
    }
    if (curr->y_hit) {
      curr->dy = 0;
      curr->dx *= FRICTION;
      dy *= FRICTION;
    }
    if (curr->x_hit) {
      dx *= FRICTION;
      curr->dx = 0;
    } else {
      curr->dx *= AIR_RESISTANCE;
    }
    ent->x += dx;
    ent->y += dy;

    //Move to next
    curr = curr->next;
  }
}

phys_object_t * physics_add_entity(entity_t * ent) {
  phys_object_t * new;
  new = malloc(sizeof(phys_object_t));
  new->ent = ent;

  linkedlist_insert_after((ll_node_t*)&root.next, (ll_node_t*)new);

  return new;
}

#include "physics.h"

#include "entity.h"
#include "linkedlist.h"
#include "log.h"
#include "gamedefs.h"
#include "geometry.h"
#include "world.h"

#include <stdlib.h>
#include <string.h>
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

#define SMOOTHING_OFFSET 0.01
#define AIR_RESISTANCE 0.99
#define FRICTION 0.5

void physics_tick(struct world * world, double delta) {
  phys_object_t * curr, * collider;
  rectangle_t temp, cresult, other_rect;
  entity_t * ent;
  int xmin,ymin,xmax,ymax;
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
    temp.w = ent->rect.w;
    temp.h = ent->rect.h;
    //
    // Test y motion
    //
    xmin = (int)(ent->rect.x + SMOOTHING_OFFSET);
    xmax = (int)(ent->rect.x + ent->rect.w - SMOOTHING_OFFSET);
    ymin = (int)(ent->rect.y + dy + SMOOTHING_OFFSET);
    ymax = (int)(ent->rect.y + ent->rect.h + dy - SMOOTHING_OFFSET);
    for (x = xmin; x <= xmax; ++x) {
      if (world_get_tile(world, x, ymin) == TILE_SOLID) {
        curr->y_hit = HIT_BOTTOM;
        curr->last_y_hit_frame = frame;
        ent->air_jumps_used = 0;
        dy = 0;
        ent->rect.y = ymin + 1 + SMOOTHING_OFFSET;
      }
      if (world_get_tile(world, x, ymax) == TILE_SOLID) {
        curr->y_hit = HIT_TOP;
        curr->last_y_hit_frame = frame;
        dy = 0;
        ent->rect.y = ymin - SMOOTHING_OFFSET;
      }
      collider = root.next;
      temp.x = ent->rect.x;
      temp.y = ent->rect.y + dy;
      while (collider) {
        //skip self
        if (collider == curr) {
          collider = collider->next;
          continue;
        }
        other_rect = collider->ent->rect;
        rectangle_intersect(&temp, &other_rect, &cresult);
        if (cresult.h > 0 && cresult.w > 0) {
          if (temp.y > other_rect.y) {
            curr->y_hit = HIT_BOTTOM;
            curr->last_y_hit_frame = frame;
            ent->rect.y = other_rect.y + other_rect.h + SMOOTHING_OFFSET;
          } else {
            curr->y_hit = HIT_TOP;
            curr->last_y_hit_frame = frame;
            ent->rect.y = other_rect.y - ent->rect.h - SMOOTHING_OFFSET;
          }
          collider->dy = curr->dy / 2;
          dy = 0;
        }
        collider = collider->next;
      }
    }
    //
    // Test X motion
    //
    xmin = (int)(ent->rect.x + dx + SMOOTHING_OFFSET);
    xmax = (int)(ent->rect.x + ent->rect.w + dx - SMOOTHING_OFFSET);
    ymin = (int)(ent->rect.y + SMOOTHING_OFFSET);
    ymax = (int)(ent->rect.y + ent->rect.h - SMOOTHING_OFFSET);
    for (y = ymin; y <= ymax; ++y) {
      if (world_get_tile(world, xmin, y) == TILE_SOLID) {
        curr->x_hit = HIT_LEFT;
        curr->last_x_hit_frame = frame;
        dx = 0;
        ent->rect.x = xmin + 1 + SMOOTHING_OFFSET;
      }
      if (world_get_tile(world, xmax, y) == TILE_SOLID) {
        curr->x_hit = HIT_RIGHT;
        curr->last_x_hit_frame = frame;
        dx = 0;
        ent->rect.x = xmin - SMOOTHING_OFFSET;
      }
      collider = root.next;
      temp.x = ent->rect.x + dx;
      temp.y = ent->rect.y;
      while (collider) {
        //skip self
        if (collider == curr) {
          collider = collider->next;
          continue;
        }
        other_rect = collider->ent->rect;
        rectangle_intersect(&temp, &other_rect, &cresult);
        if (cresult.h > 0 && cresult.w > 0) {
          if (temp.x > other_rect.x) {
            curr->x_hit = HIT_RIGHT;
            curr->last_x_hit_frame = frame;
            ent->rect.x = other_rect.x + other_rect.w + SMOOTHING_OFFSET;
          } else {
            curr->x_hit = HIT_LEFT;
            curr->last_x_hit_frame = frame;
            ent->rect.x = other_rect.x - ent->rect.w - SMOOTHING_OFFSET;
          }
          collider->dx = curr->dx / 2;
          dx = 0;
        }
        collider = collider->next;
      }
    }
    if (curr->y_hit) { curr->dy = 0; }
    if (curr->x_hit) { curr->dx = 0; }
    ent->rect.x += dx;
    ent->rect.y += dy;

    if (curr->y_hit) {
      curr->dx *= FRICTION;
    } else {
      curr->dx *= AIR_RESISTANCE;
    }
    //Move to next
    curr = curr->next;
  }
}

phys_object_t * physics_add_entity(entity_t * ent) {
  phys_object_t * new;
  new = malloc(sizeof(phys_object_t));
  memset(new, 0, sizeof(phys_object_t));
  new->ent = ent;

  linkedlist_insert_after((ll_node_t*)&root.next, (ll_node_t*)new);

  return new;
}

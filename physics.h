#ifndef _PHYSICS_H_
#define _PHYSICS_H_

struct entity;
struct world;

#define HIT_TOP 1
#define HIT_BOTTOM 2
#define HIT_LEFT 1
#define HIT_RIGHT 2

typedef struct phys_object {
  struct phys_object * next, * prev;
  struct entity * ent;
  float dx, dy;
  int last_y_hit_frame;
  int last_x_hit_frame;
  char y_hit;
  char x_hit;
} phys_object_t;

void physics_tick(struct world * world, double delta);
phys_object_t * physics_add_entity(struct entity * ent);

#endif

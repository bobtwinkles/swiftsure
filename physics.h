#ifndef _PHYSICS_H_
#define _PHYSICS_H_

struct entity;
struct world;

typedef struct phys_object {
  struct phys_object * next, * prev;
  struct entity * ent;
  float dx, dy;
  char y_hit;
  char x_hit;
} phys_object_t;

void physics_tick(struct world * world, double delta);
phys_object_t * physics_add_entity(struct entity * ent);

#endif

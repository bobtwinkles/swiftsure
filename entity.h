#ifndef _ENTITY_H_
#define _ENTITY_H_

typedef struct entity {
  float x, y;
  float w, h;
  int air_jumps_used;
  int air_jumps_max;
} entity_t;

#endif

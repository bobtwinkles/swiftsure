#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "geometry.h"

typedef struct entity {
  rectangle_t rect;
  float r,g,b;
  int air_jumps_used;
  int air_jumps_max;
} entity_t;

#endif

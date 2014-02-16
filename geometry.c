#include "geometry.h"

#include "log.h"

void validate_rectangle(rectangle_t * rect) {
  if (rect->w < 0) {
    rect->x += rect->w;
    rect->w *= -1;
  }
  if (rect->h < 0) {
    rect->y += rect->h;
    rect->h *= -1;
  }
}

void rectangle_intersect(const rectangle_t * r1, const rectangle_t * r2, rectangle_t * out) {
  out->w = -1;
  out->h = -1;
  out->x = 0;
  out->y = 0;
  if (r2->x > r1->x) {
    out->x = r2->x;
    out->w = r1->x + r1->w - r2->x;
  } else {
    out->x = r1->x;
    out->w = r2->x + r2->w - r1->x;
  }
  if (r2->y > r1->y) {
    out->y = r2->y;
    out->h = r1->y + r1->h - r2->y;
  } else {
    out->y = r1->y;
    out->h = r2->y + r2->h - r1->y;
  }
}

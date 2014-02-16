#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

typedef struct point {
  float x, y;
} point_t;

typedef struct rectangle {
  float x, y;
  float w, h;
} rectangle_t;

typedef struct circle {
  point_t center;
  float r;
} circle_t;

//Fix negative w/h
extern void validate_rectangle (rectangle_t * rect);

extern void rectangle_intersect(const rectangle_t * r1, const rectangle_t * r2, rectangle_t * out);

#endif

#ifndef _RENDERING_H_
#define _RENDERING_H_

extern int screen_width;
extern int screen_height;

struct world;

void rendering_init();

void render_world(struct world * world, float scale);

#endif

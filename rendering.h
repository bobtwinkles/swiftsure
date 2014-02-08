#ifndef _RENDERING_H_
#define _RENDERING_H_

extern int screen_width;
extern int screen_height;

struct world;

extern int rendering_init();

extern void render_world(struct world * world);

extern void render_set_camera(float x, float y, float scale);

#endif

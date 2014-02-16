#ifndef _GAME_H_
#define _GAME_H_

extern void game_init(void);

extern void game_tick(double delta);

extern void game_get_avg_pos(float * x, float * y);

extern void game_render_players(void);

#endif

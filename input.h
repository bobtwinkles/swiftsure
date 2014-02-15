#ifndef _INPUT_H_
#define _INPUT_H_

#define MAX_PLAYER 1

struct phys_object;

void input_set_player(int idx, struct phys_object * object);

void handle_events(void);

#endif

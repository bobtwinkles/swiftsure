#ifndef _ACTION_H_
#define _ACTION_H_

#define ACTION_UP       1
#define ACTION_DOWN     2
#define ACTION_LEFT     3
#define ACTION_RIGHT    4
#define ACTION_JUMP     5
#define ACTION_NORMAL   6
#define ACTION_SPECIAL  7
#define ACTION_SHIELD   8
#define ACTION_GRAB     9

struct phys_object;

void action_init(void);
void action_perform(void);
void action_enqueue(int player, int action);
void action_set_player(int index, struct phys_object * player);

#endif

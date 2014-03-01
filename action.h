#ifndef _ACTION_H_
#define _ACTION_H_

//XXX: input.c assumes no action has ID 0
#define ACTION_INVALID         0

#define ACTION_START_UP        1
#define ACTION_START_DOWN      2
#define ACTION_START_LEFT      3
#define ACTION_START_RIGHT     4
#define ACTION_START_JUMP      5
#define ACTION_START_NORMAL    6
#define ACTION_START_SPECIAL   7
#define ACTION_START_SHIELD    8
#define ACTION_START_GRAB      9

#define ACTION_ENDS_START     10

#define ACTION_END_UP         10
#define ACTION_END_DOWN       11
#define ACTION_END_LEFT       12
#define ACTION_END_RIGHT      13
#define ACTION_END_JUMP       14
#define ACTION_END_NORMAL     15
#define ACTION_END_SPECIAL    16
#define ACTION_END_SHIELD     17
#define ACTION_END_GRAB       18

struct phys_object;

void action_init(void);
void action_perform(void);
void action_enqueue(int player, int action);
void action_set_player(int index, struct phys_object * player);

#endif

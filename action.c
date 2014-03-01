#include "action.h"

#include "log.h"
#include "physics.h"
#include "entity.h"
#include "gamedefs.h"

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define DI_DESIRED_VELOCITY 40

#define DI_LEFT 0
#define DI_RIGHT 1

typedef struct action {
  int player, action;
} action_t;

typedef struct input_state {
  frame_t up;
  frame_t down;
  frame_t left;
  frame_t right;
  frame_t shield;
  frame_t jump;
  frame_t normal;
  frame_t special;
  frame_t grab;
} input_state_t;

static phys_object_t *players[MAX_PLAYERS];
static input_state_t input_states[MAX_PLAYERS];

static void jump_player(int index);
static void di_player(int index, int direction);

void action_init(void) {
}

void action_perform(void) {
  int player;
  input_state_t state;
  for (player = 0; player < MAX_PLAYERS; ++player) {
    state = input_states[player];
    if (state.jump != -1 && state.jump == frame) {
      jump_player(player);
    }
    if (state.left != -1 && state.left < frame) {
      di_player(player, DI_LEFT);
    } else if (state.right != -1 && state.right < frame) {
      di_player(player, DI_RIGHT);
    }
  }
}

void action_set_player(int idx, struct phys_object * object) {
  players[idx] = object;
  memset(input_states + idx, -1, sizeof(input_state_t));
}

void action_enqueue(int player, int action) {
  swiftsure_log(DEBUG, "enqueue %d %d\n", player, action);
  switch(action) {
    case ACTION_START_UP:      input_states[player].up = frame; break;
    case ACTION_START_DOWN:    input_states[player].down = frame; break;
    case ACTION_START_LEFT:    input_states[player].left = frame; break;
    case ACTION_START_RIGHT:   input_states[player].right = frame; break;
    case ACTION_START_JUMP:    input_states[player].jump = frame; break;
    case ACTION_START_NORMAL:  input_states[player].normal = frame; break;
    case ACTION_START_SPECIAL: input_states[player].special = frame; break;
    case ACTION_START_SHIELD:  input_states[player].shield = frame; break;
    case ACTION_START_GRAB:    input_states[player].grab = frame; break;
    case ACTION_END_UP:        input_states[player].up = -1; break;
    case ACTION_END_DOWN:      input_states[player].down = -1; break;
    case ACTION_END_LEFT:      input_states[player].left = -1; break;
    case ACTION_END_RIGHT:     input_states[player].right = -1; break;
    case ACTION_END_JUMP:      input_states[player].jump = -1; break;
    case ACTION_END_NORMAL:    input_states[player].normal = -1; break;
    case ACTION_END_SPECIAL:   input_states[player].special = -1; break;
    case ACTION_END_SHIELD:    input_states[player].shield = -1; break;
    case ACTION_END_GRAB:      input_states[player].grab = -1; break;
    default:
      swiftsure_log(
          DEBUG, "unknown action %d from player %d\n", action, player);
  }
}

static void di_player(int index, int direction) {
  if (direction == DI_RIGHT) {
    if (players[index]->dx > -DI_DESIRED_VELOCITY) {
      float diff = -DI_DESIRED_VELOCITY - players[index]->dx;
      players[index]->dx += diff / 8;
    }
  } else {
    if (players[index]->dx < DI_DESIRED_VELOCITY) {
      float diff = DI_DESIRED_VELOCITY - players[index]->dx;
      players[index]->dx += diff / 8;
    }
  }
}

static void jump_player(int index) {
  if (frame - 5 <= players[index]->last_y_hit_frame) {
    players[index]->dy = 32;
    swiftsure_log(DEBUG, "jumpin\n");
  } else if (players[index]->ent->air_jumps_used < players[index]->ent->air_jumps_max) {
    players[index]->dy = 32;
    players[index]->ent->air_jumps_used += 1;
    swiftsure_log(DEBUG, "jumpin2\n");
  }
}

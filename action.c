#include "action.h"

#include "log.h"
#include "physics.h"
#include "entity.h"
#include "gamedefs.h"

#include <stdlib.h>
#include <string.h>

#define DI_DESIRED_VELOCITY 40

#define DI_LEFT 0
#define DI_RIGHT 1

typedef struct action {
  int player, action;
} action_t;

static action_t * action_queue;
static int actions_enqueued;
static int max_actions;

static phys_object_t *players[MAX_PLAYERS];

static void jump_player(int index);
static void di_player(int index, int direction);

void action_init(void) {
  action_queue = calloc(4, sizeof(action_t));
}

void action_perform(void) {
  int action;
  int player;
  for (action = 0; action < actions_enqueued; ++action) {
    player = action_queue[action].player;
    switch(action_queue[action].action) {
      case ACTION_UP: jump_player(player); break;
      case ACTION_LEFT: di_player(player, DI_LEFT); break;
      case ACTION_RIGHT: di_player(player, DI_RIGHT); break;
      default:
        swiftsure_log(
            DEBUG, "unknown action %d from player %d\n",
            action_queue[actions_enqueued].action, player);
    }
  }
  actions_enqueued = 0;
}

void action_set_player(int idx, struct phys_object * object) {
  players[idx] = object;
}

void action_enqueue(int player, int action) {
  action_queue[actions_enqueued] = (action_t) {
    .player = player,
    .action = action
  };
  actions_enqueued++;
  if (max_actions == actions_enqueued) {
    action_queue = realloc(action_queue, max_actions * 2);
    max_actions *= 2;
  }
}

static void di_player(int index, int direction) {
  if (direction == DI_LEFT) {
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

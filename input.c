#include "input.h"

#include <SDL.h>
#include <stdlib.h>

#include "log.h"
#include "physics.h"
#include "entity.h"
#include "globals.h"

static phys_object_t *players[MAX_PLAYER];

void input_set_player(int idx, phys_object_t * object) {
  players[idx] = object;
}

void handle_events(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_ESCAPE: exit(0); break;
          case SDLK_a: players[0]->dx = -32; break;
          case SDLK_d: players[0]->dx = 32; break;
          case SDLK_w: 
            if (frame - 5 <= players[0]->last_y_hit_frame) {
              players[0]->dy = 32;
              players[0]->ent->y += 1;
              swiftsure_log(DEBUG, "jumpin\n");
            } else if (players[0]->ent->air_jumps_used < players[0]->ent->air_jumps_max) {
              players[0]->dy = 32;
              players[0]->ent->y += 0.1;
              players[0]->ent->air_jumps_used += 1;
              swiftsure_log(DEBUG, "jumpin2\n");
            }
            break;
        }
        break;
    }
  }
}

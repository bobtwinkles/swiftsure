#include "input.h"

#include <SDL.h>
#include <stdlib.h>

#include "log.h"
#include "physics.h"
#include "entity.h"
#include "gamedefs.h"

#define FINAL_BUTTON 0
#define SP_ATTACK_BUTTON 1
#define JUMP_BUTTON 2
#define NORM_ATTACK_BUTTON 3

#define DI_DESIRED_VELOCITY 40

#define DI_LEFT 0
#define DI_RIGHT 1

typedef struct joystick_status {
  SDL_Joystick * joystick;
  int x_deflection;
  int y_deflection;
  char button_status[4];
} joystick_status_t;

static phys_object_t *players[MAX_PLAYERS];
static joystick_status_t joysticks[MAX_PLAYERS];
static Uint8 * keyboard_state;
static char p1_should_kbd;

static int translate_button(int joybutton);
static void jump_player(int index);
static void di_player(int index, int direction);

int input_init(void) {
  int i;
  int max = SDL_NumJoysticks();

  swiftsure_log(DEBUG, "There are %d joysticks availible\n", max);

  if (max < 1) {
    swiftsure_log(DEBUG, "SDL couldn't find any joysticks =(\n");
    swiftsure_log(DEBUG, "SDL error: %s\n", SDL_GetError());
    return -1;
  }

  if (max > MAX_PLAYERS) { max = MAX_PLAYERS; }
  else if (max == MAX_PLAYERS - 1) {p1_should_kbd = 1; }
  if (p1_should_kbd) {
    swiftsure_log(DEBUG, "Player 1 needs to use the keyboard\n");
  }
  for (i = 0; i < SDL_NumJoysticks(); ++i) {
    const char * err;
    joysticks[i].joystick = SDL_JoystickOpen(i);

    if (joysticks[i].joystick == NULL) {
      swiftsure_log(DEBUG, "Couldn't init joystick\n");
      return -1;
    }
  }
  keyboard_state = SDL_GetKeyboardState(NULL);
  return 0;
}

void input_set_player(int idx, phys_object_t * object) {
  players[idx] = object;
}

void handle_events(void) {
  SDL_Event event;
  int i;

  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_ESCAPE: exit(0); break;
        }
        break;
      case SDL_JOYAXISMOTION:
        switch(event.jaxis.axis) {
          case 0:
            joysticks[event.jaxis.which].x_deflection = event.jaxis.value;
            break;
          case 1:
            joysticks[event.jaxis.which].y_deflection = event.jaxis.value; break;
        }
        break;
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        joysticks[event.jbutton.which].button_status[translate_button(event.jbutton.button)] = event.jbutton.state;
        if (event.jbutton.state == SDL_PRESSED) {
          switch (translate_button(event.jbutton.button)) {
            case JUMP_BUTTON:
              jump_player(event.jbutton.which + p1_should_kbd);
              break;
          }
        }
        break;
    }
  }
  //DI
  if (p1_should_kbd) {
    if (keyboard_state[SDL_SCANCODE_W]) {
      jump_player(0);
    }
    if (keyboard_state[SDL_SCANCODE_A]) {
      di_player(0, DI_LEFT);
    }
    if (keyboard_state[SDL_SCANCODE_D]) {
      di_player(0, DI_RIGHT);
    }
  }
  for (i = p1_should_kbd; i < MAX_PLAYERS; ++i) {
    if (players[i] != NULL) {
      joystick_status_t status = joysticks[i - p1_should_kbd];
      if (status.x_deflection > 10000) {
        di_player(i, DI_RIGHT);
      } else if (status.x_deflection < -10000) {
        di_player(i, DI_LEFT);
      }
    }
  }
}

static int translate_button (int joybutton) {
  return joybutton - 12;
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

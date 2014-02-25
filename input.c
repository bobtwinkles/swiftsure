#include "input.h"

#include <SDL.h>
#include <stdlib.h>

#include "log.h"
#include "gamedefs.h"

#define FINAL_BUTTON 0
#define SP_ATTACK_BUTTON 1
#define JUMP_BUTTON 2
#define NORM_ATTACK_BUTTON 3

typedef struct joystick_info {
  SDL_Joystick * joystick;
  SDL_JoystickID id;
} joystick_info_t;

typedef struct binding {
  int player;
  int action;
} binding_t;

static const Uint8 * keyboard_state;
static joystick_info_t * joysticks;

int input_init(void) {
  int num_joysticks = SDL_NumJoysticks();

  swiftsure_log(DEBUG, "There are %d joysticks availible\n", num_joysticks);

  if (num_joysticks < 1) {
    swiftsure_log(DEBUG, "SDL couldn't find any joysticks =(\n");
    swiftsure_log(DEBUG, "SDL error: %s\n", SDL_GetError());
    return -1;
  }

  if (MAX_PLAYERS - 1 > num_joysticks) {
    swiftsure_log(DEBUG, "Not enough input devices: plug in more gamepads!\n");
    swiftsure_log(DEBUG, "Want: %d have %d\n", MAX_PLAYERS - 1, num_joysticks);
    return -1;
  }

  joysticks = NULL;
  input_scan_joysticks();

  keyboard_state = SDL_GetKeyboardState(NULL);
  return 0;
}

void input_scan_joysticks(void) {
  int i;
  int num_joysticks = SDL_NumJoysticks();
  if (joysticks == NULL) {
    joysticks = malloc(num_joysticks * sizeof(joystick_info_t));
  }

  for (i = 0; i < SDL_NumJoysticks(); ++i) {
    const char * err;
    joysticks[i].joystick = SDL_JoystickOpen(i);

    err = SDL_GetError();
    if (joysticks[i].joystick == NULL || *err != '\00') {
      swiftsure_log(DEBUG, "Couldn't init joystick %i, error %s\n", i, err);
    }
  }
}

void handle_events(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
          case SDLK_ESCAPE: exit(0); break;
        }
        break;
      case SDL_JOYAXISMOTION:
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        break;
    }
  }
}

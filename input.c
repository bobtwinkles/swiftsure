#include "input.h"

#include <SDL.h>
#include <stdlib.h>
#include <inttypes.h>

#include "action.h"
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

static const Uint8 * keyboard_state;
static joystick_info_t * joysticks;

/*****************************************************************************\
 *                      KEYBINDING MANGEMENT INTERFACE                       *
\*****************************************************************************/

#define EVENT_TYPE_JOYBUTTON 2
#define EVENT_TYPE_JOYSTICK 1
#define EVENT_TYPE_KEYBOARD 0

typedef struct keyboard_event {
  int player;
  int press_action;
  int release_action;
  uint16_t keycode;
} keyboard_event_t;

typedef struct joystick_event {
  SDL_JoystickID stick;
  int action_min_pass;
  int action_min_fail;
  int action_max_pass;
  int action_max_fail;
  uint8_t axis;
  int16_t threshold;
} joystick_event_t;

typedef struct joybutton_event {
  SDL_JoystickID stick;
  int press_action;
  int release_action;
  int button;
} joybutton_event_t;

static void bind_events();

static void bind_keyboard_event(keyboard_event_t event);
static void bind_joybutton_event(joybutton_event_t event);
static void bind_joystick_event(joystick_event_t event);
static void bind_joystick_to_player(SDL_JoystickID ID, int player);

static void perform_action_for_keyboard(
      uint16_t keycode, int state
    );
static void perform_action_for_joystick(
      SDL_JoystickID stick, uint8_t axis,
      int16_t deflection
    );
static void perform_action_for_joybutton(
      SDL_JoystickID stick,
      uint8_t button, int state
    );

/*****************************************************************************\
 *                    END KEYBINDING MANGEMENT INTERFACE                     *
\*****************************************************************************/

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

  bind_events();

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
      case SDL_KEYUP:
        if (!event.key.repeat) {
          perform_action_for_keyboard(event.key.keysym.sym, event.key.state);
        }
        break;
      case SDL_JOYAXISMOTION:
        perform_action_for_joystick(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
        break;
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        perform_action_for_joybutton(event.jbutton.which, event.jbutton.button, event.jbutton.state);
        break;
    }
  }
}

void bind_events() {
  keyboard_event_t kb;
  joystick_event_t js;
  joybutton_event_t jb;

#define KBD(p, r, k) kb = (keyboard_event_t) {.player = 0, .press_action = p,\
    .release_action = r, .keycode = k}; bind_keyboard_event(kb)

  KBD(ACTION_START_UP, ACTION_END_UP, SDLK_w);
  KBD(ACTION_START_DOWN, ACTION_END_DOWN, SDLK_s);
  KBD(ACTION_START_LEFT, ACTION_END_LEFT, SDLK_d);
  KBD(ACTION_START_RIGHT, ACTION_END_RIGHT, SDLK_a);
  KBD(ACTION_START_JUMP, ACTION_END_JUMP, SDLK_SPACE);

#define JB(s, p, r, b) jb = (joybutton_event_t) { .stick = s, .press_action = p,\
    .release_action = r, .button = b}; bind_joybutton_event(jb)

  bind_joystick_to_player(0, 0);
  bind_joystick_to_player(1, 1);
  JB(0, ACTION_START_JUMP, ACTION_END_JUMP, 14);
  JB(1, ACTION_START_JUMP, ACTION_END_JUMP, 14);

#define JS(s, a, t, minp, minf, maxp, maxf) js = (joystick_event_t) {\
    .stick = s,\
    .action_min_pass = minp, .action_min_fail = minf,\
    .action_max_pass = maxp, .action_max_fail = maxf,\
    .axis = a, .threshold = t}; bind_joystick_event(js)

  JS(0, 0, 1000, ACTION_START_RIGHT, ACTION_END_RIGHT, ACTION_START_LEFT, ACTION_END_LEFT);
  JS(0, 1, 1000, ACTION_START_UP, ACTION_END_UP, ACTION_START_DOWN, ACTION_END_DOWN);
  JS(1, 0, 1000, ACTION_START_RIGHT, ACTION_END_RIGHT, ACTION_START_LEFT, ACTION_END_LEFT);
  JS(1, 1, 1000, ACTION_START_UP, ACTION_END_UP, ACTION_START_DOWN, ACTION_END_DOWN);

#undef KBD
#undef JB
#undef JS
}

/*****************************************************************************\
 *                      KEYBINDING MANGEMENT INTERFACE                       *
\*****************************************************************************/

#define NUM_BINDINGS_BUCKETS 32

joybutton_event_t jb_events[NUM_BINDINGS_BUCKETS];
unsigned int num_jbevents;
joystick_event_t js_events[NUM_BINDINGS_BUCKETS];
unsigned int num_jsevents;
keyboard_event_t kb_events[NUM_BINDINGS_BUCKETS];
unsigned int num_kbevents;
int joystick_player_map [NUM_BINDINGS_BUCKETS];

static void bind_keyboard_event(keyboard_event_t event) {
  swiftsure_log(DEBUG, "Binding key with code %d to action %d %d on player %d\n"
               ,event.keycode, event.press_action, event.release_action, event.player);
  if (num_kbevents == NUM_BINDINGS_BUCKETS) {
    swiftsure_log(CRIT, "Failed to bind keybinding\n");
    return;
  }
  kb_events[num_kbevents++] = event;
}

static void bind_joybutton_event(joybutton_event_t event) {
  swiftsure_log(DEBUG, "Binding button %d on joystick %d to action %d %d on player %d\n"
               ,event.button, event.stick, event.press_action, event.release_action, joystick_player_map[event.stick]);
  if (num_jbevents == NUM_BINDINGS_BUCKETS) {
    swiftsure_log(CRIT, "Failed to bind joybutton binding\n");
    return;
  }
  jb_events[num_jbevents++] = event;
}

static void bind_joystick_event(joystick_event_t event) {
  if (num_jsevents == NUM_BINDINGS_BUCKETS) {
    swiftsure_log(CRIT, "Failed to bind keybinding\n");
    return;
  }
  js_events[num_jsevents++] = event;
}

static void bind_joystick_to_player(SDL_JoystickID ID, int player) {
  joystick_player_map[ID] = player;
}

static void perform_action_for_keyboard(
      uint16_t keycode, int state
    ) {
  int i;
  for (i = 0; i < num_kbevents; ++i) {
    if (kb_events[i].keycode == keycode) {
      if (state) {
        action_enqueue(kb_events[i].player, kb_events[i].press_action);
      } else {
        action_enqueue(kb_events[i].player, kb_events[i].release_action);
      }
    }
  }
}

static void perform_action_for_joystick(
      SDL_JoystickID stick, uint8_t axis,
      int16_t deflection
    ) {
  int i;
  for (i = 0; i < num_jsevents; ++i) {
    if (js_events[i].stick == stick && js_events[i].axis == axis) {
      if (deflection < -js_events[i].threshold) {
        action_enqueue(joystick_player_map[stick], js_events[i].action_min_pass);
      } else {
        action_enqueue(joystick_player_map[stick], js_events[i].action_min_fail);
      }
      if (deflection > js_events[i].threshold) {
        action_enqueue(joystick_player_map[stick], js_events[i].action_max_pass);
      } else {
        action_enqueue(joystick_player_map[stick], js_events[i].action_max_fail);
      }
    }
  }
}

static void perform_action_for_joybutton(
      SDL_JoystickID stick,
      uint8_t button, int state
    ) {
  int i;
  for (i = 0; i < num_jbevents; ++i) {
    if (jb_events[i].stick == stick && jb_events[i].button == button) {
      if (state) {
        action_enqueue(joystick_player_map[stick], jb_events[i].press_action);
      } else {
        action_enqueue(joystick_player_map[stick], jb_events[i].release_action);
      }
    }
  }
}

/*****************************************************************************\
 *                    END KEYBINDING MANGEMENT INTERFACE                     *
\*****************************************************************************/

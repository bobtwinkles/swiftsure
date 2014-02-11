#include "input.h"

#include <SDL.h>
#include <stdlib.h>

#include "log.h"

void handle_events(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: exit(0); break;
    }
  }
}

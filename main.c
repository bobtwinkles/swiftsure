#include <stdio.h>

#include "rendering.h"
#include "world.h"
#include "log.h"

world_t world;

int main(int argc, const char ** argv) {
  if (swiftsure_log_init() < 0) {
    printf("Bad times, we couldn't open our log file =(\n");
    return -1;
  }

  rendering_init();

  world.width = WORLD_SIZE;
  world.height = WORLD_SIZE;

  world_init(&world);

  swiftsure_log(0, "Startin engines\n");

  while (1) {
      render_world(&world, 2);
  }
}

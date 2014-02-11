#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "input.h"
#include "rendering.h"
#include "world.h"
#include "log.h"

world_t world;

static int frame;

int main(int argc, const char ** argv) {
  struct timeval start_time, end_time;
  struct timespec tspec = {0, 0};
  struct timespec left = {0, 0};
  int error;
  long long elapsed;

  if (swiftsure_log_init() < 0) {
    printf("Bad times, we couldn't open our log file =(\n");
    return -1;
  }

  rendering_init();

  world.width = WORLD_SIZE;
  world.height = WORLD_SIZE;

  world_init(&world, 1);

  swiftsure_log(INFO, "Startin engines\n");

  render_set_camera(-screen_width / 4, -screen_height / 2, 2);

  frame = 0;

  while (1) {
    gettimeofday(&start_time, NULL);

    render_set_camera(-screen_width / 4, -screen_height / 2, 2);

    handle_events();
    render_world(&world);
    ++frame;

    if (frame % 20 == 1) {
      swiftsure_log(DEBUG, "regen stage\n");
      world_destroy(&world);
      world_init(&world, frame);
    }

    //Rate limiting
    gettimeofday(&end_time, NULL);
    elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
    if (elapsed < 16666) {
      tspec.tv_nsec = (16666 - elapsed) * 1000;
      error = nanosleep(&tspec, &left);
      if (error < 0) {
        swiftsure_log(DEBUG, "We had %d seconds and %d nanoseconds left to sleep, errno %d\n", left.tv_sec, left.tv_nsec, errno);
      }
    }
  }
}

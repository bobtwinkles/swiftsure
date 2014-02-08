#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#include "rendering.h"
#include "world.h"
#include "log.h"

world_t world;

static int frame;

int main(int argc, const char ** argv) {
  struct timeval start_time, end_time;
  struct timespec tspec;
  long long elapsed;

  if (swiftsure_log_init() < 0) {
    printf("Bad times, we couldn't open our log file =(\n");
    return -1;
  }

  rendering_init();

  world.width = WORLD_SIZE;
  world.height = WORLD_SIZE;

  world_init(&world);

  swiftsure_log(0, "Startin engines\n");

  render_set_camera(-screen_width / 4, -screen_height / 2, 2);

  while (1) {
    gettimeofday(&start_time, NULL);

    render_set_camera((frame)-screen_width / 4, -screen_height / 2, 2);

    render_world(&world);
    ++frame;

    gettimeofday(&end_time, NULL);

    elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec -start_time.tv_usec);
    if (elapsed < 16666) {
      tspec.tv_nsec = elapsed * 1000;
      //I mean, we _could_ handle errors... NAH, HOLD MY BEER CAUSE I GOT THIS
      nanosleep(&tspec, NULL);
    }
  }
}

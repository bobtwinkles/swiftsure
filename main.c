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
  struct timespec tspec = {0, 0};
  struct timespec left = {0, 0};
  long long elapsed;

  if (swiftsure_log_init() < 0) {
    printf("Bad times, we couldn't open our log file =(\n");
    return -1;
  }

  rendering_init();

  world.width = WORLD_SIZE;
  world.height = WORLD_SIZE;

  world_init(&world, 1);

  swiftsure_log(0, "Startin engines\n");

  render_set_camera(-screen_width / 4, -screen_height / 2, 2);

  while (1) {
    gettimeofday(&start_time, NULL);

    render_set_camera(-screen_width / 4, -screen_height / 2, 2);

    render_world(&world);
    ++frame;

    if (frame % 10 == 1) {
      swiftsure_log(DEBUG, "regen stage\n");
      world_destroy(&world);
      world_init(&world, frame);
    }

    gettimeofday(&end_time, NULL);

    elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec);
    swiftsure_log(DEBUG, "Frame took %d microseconds\n", elapsed);
    if (elapsed < 16666) {
      tspec.tv_nsec = (16666 - elapsed) * 1000;
      swiftsure_log(DEBUG, "We have %d seconds and %d nanoseconds to sleep\n", tspec.tv_sec, tspec.tv_nsec);
      //I mean, we _could_ handle errors... NAH, HOLD MY BEER CAUSE I GOT THIS
      nanosleep(&tspec, &left);
      swiftsure_log(DEBUG, "We had %d seconds and %d nanoseconds left to sleep\n", left.tv_sec, left.tv_nsec);
    }
  }
}

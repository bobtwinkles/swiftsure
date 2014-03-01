#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "action.h"
#include "input.h"
#include "rendering.h"
#include "world.h"
#include "physics.h"
#include "log.h"
#include "game.h"
#include "gamedefs.h"

world_t world;

int main(int argc, const char ** argv) {
  struct timeval start_time, end_time;
  struct timespec tspec = {0, 0};
  struct timespec left = {0, 0};
  int error;
  long long elapsed;
  float cam_center_x, cam_center_y;

  if (swiftsure_log_init() < 0) {
    printf("Bad times, we couldn't open our log file =(\n");
    return -1;
  }

  rendering_init();

  if (input_init() < 0) {
    printf("Bad times, we couldn't initialize the input subsystem\n");
    return -1;
  }
  action_init();

  world.width = WORLD_SIZE;
  world.height = WORLD_SIZE;

  world_init(&world, 1);
  game_init();

  swiftsure_log(INFO, "Startin engines\n");

  frame = 0;
  elapsed = 16666;

  while (1) {
    gettimeofday(&start_time, NULL);

    game_get_avg_pos(&cam_center_x, &cam_center_y);
    render_set_camera(-cam_center_x, -cam_center_y, 4);

    handle_events();
    action_perform();
    render_start_frame();
    render_world(&world);
    game_render_players();
    render_end_frame();
    ++frame;

    physics_tick(&world, 10. / elapsed);

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

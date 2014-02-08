#include "rendering.h"

#include "log.h"
#include "world.h"

#include <curses.h>

int screen_width;
int screen_height;

static float cam_x;
static float cam_y;
static float cam_scale;

int rendering_init(void) {
  initscr();
  if (!has_colors()) {
    printf("We can't change color =(\n");
    endwin();
    return -1;
  }
  start_color();
  cbreak();
  noecho();
  curs_set(0); //Hide cursor

  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, screen_height, screen_width);

  swiftsure_log(INFO, "We have %d color pairs availible.\n", COLOR_PAIRS);
  //initialize colors
  init_pair(TILE_AIR  , COLOR_BLACK, COLOR_BLACK);
  init_pair(TILE_SOLID, COLOR_WHITE, COLOR_WHITE);

  return 0;
}

void render_world(world_t * world) {
  int x, y;
  int cur_color = TILE_INVALID;
  for (x = 0; x < screen_width; ++x) {
    for (y = 0; y < screen_height; ++y) {
      int xx, yy;
      xx = (int)(cam_scale * (x + cam_x)) + world->width / 2;
      yy = (int)(cam_scale * (y + cam_y)) + world->height / 2;
      tile_t tile = world_get_tile(world, xx, yy);

      if (cur_color != tile) {
        if (cur_color != TILE_INVALID) {
          attroff(COLOR_PAIR(cur_color));
        }
        cur_color = tile;
        if (cur_color != TILE_INVALID) {
          attron (COLOR_PAIR(cur_color));
        }
      }

      if (tile != TILE_INVALID) {
        mvaddstr(y, x * 2, "  ");
      }
    }
  }
  refresh();
}

void render_set_camera(float x, float y, float scale) {
  cam_x = x;
  cam_y = y;
  cam_scale = scale;
}

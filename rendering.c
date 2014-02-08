#include "rendering.h"

#include "world.h"

#include <curses.h>

int screen_width;
int screen_height;

static int frame;

static const char * tile_chars = "  []";

void rendering_init(void) {
  initscr();
  cbreak();
  noecho();

  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, screen_height, screen_width);
}

void render_world(world_t * world, float scale) {
  int x, y;
  for (x = 0; x < world->width; ++x) {
    for (y = 0; y < world->height; ++y) {
      int xx, yy;
      xx = (int)(scale * (x + frame / 100));
      yy = (int)(scale * (y + frame / 100));
      tile_t tile = world_get_tile(world, xx, yy);

      if (tile != TILE_INVALID) {
        mvaddch(y, x * 2 + 0, tile_chars[tile * 2 + 0]);
        mvaddch(y, x * 2 + 1, tile_chars[tile * 2 + 1]);
      }
    }
  }
//  frame += 1;
  refresh();
}

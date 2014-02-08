#include "rendering.h"

#include "log.h"
#include "world.h"

#include <curses.h>

int screen_width;
int screen_height;

static float cam_x;
static float cam_y;
static float cam_scale;

//Color offsets
//XXX: CANNOT CHANGE TILE OFFSET WITHOUT BREAKING TILE RENDERING
#define COLORS_TILE      0
#define COLORS_LOG       4
#define COLORS_CHARACTER 16

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
  init_pair(COLORS_TILE + TILE_AIR  , COLOR_BLACK, COLOR_BLACK);
  init_pair(COLORS_TILE + TILE_SOLID, COLOR_WHITE, COLOR_WHITE);

  init_pair(COLORS_LOG + DEBUG, COLOR_GREEN , COLOR_BLACK);
  init_pair(COLORS_LOG + INFO , COLOR_WHITE , COLOR_BLACK);
  init_pair(COLORS_LOG + WARN , COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLORS_LOG + CRIT , COLOR_RED   , COLOR_BLACK);

  return 0;
}

void render_world(world_t * world) {
  int x, y, i;
  int cur_color;

  //Log printing
  int log_length = swiftsure_log_get_length();
  if (log_length > 0) {
    char buffer[MAX_LOG_LINE_LENGTH + 5];
    cur_color = COLORS_LOG + swiftsure_log_get_message(0)->level;
    attron(COLOR_PAIR(cur_color));
    for (i = 0; i < log_length; ++i) {
      log_data_t * data = swiftsure_log_get_message(i);
      if (cur_color != COLORS_LOG + data->level) {
        attroff(COLOR_PAIR(cur_color));
        cur_color = COLORS_LOG + data->level;
        attron(COLOR_PAIR(cur_color));
      }
      snprintf(buffer, MAX_LOG_LINE_LENGTH + 5, "%4d:%s", data->abs_index, data->msg);
      mvaddstr(i, 0, buffer);
    }
    attroff(cur_color);
  }

  cur_color = TILE_INVALID;
  for (x = 0; x < screen_width; ++x) {
    for (y = 0; y < screen_height; ++y) {
      int xx, yy;
      xx = (int)(cam_scale * (x + cam_x)) + world->width / 2;
      yy = (int)(cam_scale * (y + cam_y)) + world->height / 2;
      tile_t tile = world_get_tile(world, xx, yy);

      //XXX:CANNOT CHANGE TILE OFFSET WITHOUT BREAKING THIS
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
  //XXX:CANNOT CHANGE TILE OFFSET WITHOUT BREAKING THIS
  if (cur_color != TILE_INVALID) {
    attroff(COLOR_PAIR(cur_color));
  }
  refresh();
}

void render_set_camera(float x, float y, float scale) {
  cam_x = x;
  cam_y = y;
  cam_scale = scale;
}

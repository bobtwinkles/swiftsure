#include "rendering.h"

#include "world.h"

#include <curses.h>

int screen_width;
int screen_height;

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

void render_world(world_t * world) {
    int x, y;
    for (x = 0; x < world->width; ++x) {
        for (y = 0; y < world->height; ++y) {
            tile_t tile = world_get_tile(world, x, y);

            mvaddch(y, x * 2 + 0, tile_chars[tile * 2 + 0]);
            mvaddch(y, x * 2 + 1, tile_chars[tile * 2 + 1]);
        }
    }
    refresh();
}

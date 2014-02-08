#include "rendering.h"
#include "world.h"

world_t world;

int main(int argc, const char ** argv) {
    rendering_init();

    world.width = screen_width / 2;
    world.height = screen_height;

    world_init(&world);

    while (1) {
        render_world(&world);
    }
}

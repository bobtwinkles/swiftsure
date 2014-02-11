#include "rendering.h"

#include "log.h"
#include "world.h"

#include <SDL.h>
#include <GL/glew.h>

int screen_width;
int screen_height;

static float cam_x;
static float cam_y;
static float cam_scale;

static SDL_Window * window;
static SDL_GLContext context;

static void sdl_die(const char * message) {
  swiftsure_log(CRIT, "%s: %s\n", message, SDL_GetError());
  SDL_Quit();
}

static int check_sdl_error(const char * file, int line) {
  const char * error = SDL_GetError();
  if (*error != 0) {
    swiftsure_log(CRIT, "%s:%d%s\n", file, line, error);
    return -1;
  }
  return 0;
}

int rendering_init(const char * argv, int argc) {
  //SDL init
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    sdl_die("Couldn't initialize SDL");
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  window = SDL_CreateWindow("Swiftsure", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

  if (!window) {
    sdl_die("Couldn't create window");
    return -1;
  }

  SDL_GetWindowSize(window, &screen_width, &screen_height);

  context = SDL_GL_CreateContext(window);
  if (check_sdl_error(__FILE__, __LINE__) < 0) {
    return -1;
  }
  //vSync
  SDL_GL_SetSwapInterval(1);
  //GLEW init
  if (glewInit() != GLEW_OK) {
    swiftsure_log(CRIT, "Couldn't initialize GLEW =(");
    return -1;
  }
  //Set up OGL state that never chagnes
  glClearColor(0.f, 0.f, 0.f, 0.f);
  return 0;
}

void render_world(struct world * world) {
  int x, y;
  float xx, yy;
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();
  glBegin(GL_LINE_STRIP);
  glColor3f(1., 0, 0);
  glVertex2f(0, 1);
  glVertex2f(0, 0);
  glColor3f(0, 1., 0);
  glVertex2f(0, 0);
  glVertex2f(1, 0);
  glEnd();
  glScalef(cam_scale / screen_width, cam_scale / screen_height, 1);
  glTranslatef(cam_x * cam_scale, cam_y * cam_scale, 0);

  glBegin(GL_QUADS);

  const float offset = cam_scale;

  glColor3f(1, 1, 1);
  for (x = 0; x < world->width; ++x) {
    for (y = 0; y < world->height; ++y) {
      if (world_get_tile(world, x, y) == TILE_SOLID) {
        xx = (float)x * cam_scale;
        yy = (float)y * cam_scale;
        glVertex2f(xx, yy);
        glVertex2f(xx, yy + offset);
        glVertex2f(xx + offset, yy + offset);
        glVertex2f(xx + offset, yy);
      }
    }
  }

  glEnd();

  SDL_GL_SwapWindow(window);
}

void render_set_camera(float x, float y, float scale) {
  cam_x = x;
  cam_y = y;
  cam_scale = scale;
}

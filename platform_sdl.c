#include "platform.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

struct platform {
  SDL_Window *screen;
  SDL_Renderer *renderer;
};

platform *platform_create(const char *title, int scale) {
  (void)title;
  (void)scale;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("SDL_Init: %s", SDL_GetError());
    return NULL;
  }

  platform *p = malloc(sizeof(*p));
  if (!p) {
    SDL_Quit();
    return NULL;
  }

  SDL_CreateWindowAndRenderer(1024, 768, SDL_WINDOW_SHOWN, &p->screen,
                              &p->renderer);

  SDL_SetWindowTitle(p->screen, title);

  return p;
}

void platform_destroy(platform *p) {
  SDL_DestroyRenderer(p->renderer);
  SDL_DestroyWindow(p->screen);
  free(p);
  SDL_Quit();
}

void platform_present(platform *p, const uint32_t *videobuffer, int w, int h) {
  (void)p;
  (void)videobuffer;
  (void)w;
  (void)h;
}

bool platform_poll(platform *p, uint8_t keypad[PLATFORM_NUM_KEYS]) {
  (void)p;
  (void)keypad;

  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      return false;
    case SDL_KEYDOWN:
      if (e.key.keysym.sym == SDLK_ESCAPE)
        return false;
      break;
    }
  }
  return true;
}

void platform_beep(platform *p, bool on) {
  (void)p;
  (void)on;
  // noop
}

uint64_t platform_now_ns(void) {
  uint64_t counter = SDL_GetPerformanceCounter();
  uint64_t freq = SDL_GetPerformanceFrequency();

  return (counter / freq) * 1000000000ULL +
         ((counter % freq) * 1000000000ULL) / freq;
}

void platform_sleep_ns(uint64_t ns) { SDL_Delay((Uint32)(ns / 1000000ULL)); }

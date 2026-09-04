#include "platform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdlib.h>

struct platform {
  SDL_Window *screen;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int tex_w;
  int tex_h;
};

platform *platform_create(const char *title, int scale) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init: %s", SDL_GetError());
    return NULL;
  }

  platform *p = calloc(1, sizeof(*p));
  if (!p) {
    SDL_Quit();
    return NULL;
  }

  if (SDL_CreateWindowAndRenderer(64 * scale, 32 * scale, SDL_WINDOW_SHOWN,
                                  &p->screen, &p->renderer) != 0) {
    SDL_Log("SDL_CreateWindowAndRenderer: %s", SDL_GetError());
    free(p);
    SDL_Quit();
    return NULL;
  }

  SDL_SetWindowTitle(p->screen, title);
  SDL_SetRenderDrawColor(p->renderer, 0, 0, 0, 255);

  return p;
}

void platform_destroy(platform *p) {
  if (!p)
    return;

  if (p->texture)
    SDL_DestroyTexture(p->texture);
  SDL_DestroyRenderer(p->renderer);
  SDL_DestroyWindow(p->screen);
  free(p);
  SDL_Quit();
}

void platform_present(platform *p, const uint32_t *videobuffer, int w, int h) {
  if (!p->texture || p->tex_w != w || p->tex_h != h) {
    if (p->texture)
      SDL_DestroyTexture(p->texture);

    p->texture = SDL_CreateTexture(p->renderer,
                                   SDL_PIXELFORMAT_ARGB8888, // matches uint32_t
                                   SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!p->texture) {
      SDL_Log("SDL_CreateTexture: %s", SDL_GetError());
      p->tex_w = p->tex_h = 0;
      return;
    }
    p->tex_w = w;
    p->tex_h = h;
  }

  SDL_UpdateTexture(p->texture, NULL, videobuffer, w * (int)sizeof(uint32_t));
  SDL_RenderClear(p->renderer);
  SDL_RenderCopy(p->renderer, p->texture, NULL, NULL);
  SDL_RenderPresent(p->renderer);
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

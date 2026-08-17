#define _POSIX_C_SOURCE 199309L

#include "platform.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct platform {
  bool cursor_hidden;
};

static volatile sig_atomic_t want_quit = 0;

static void handle_sigint(int sig) {
  (void)sig;
  want_quit = 1;
}

platform *platform_create(const char *title, int scale) {
  (void)title;
  (void)scale;
  platform *p = malloc(sizeof(*p));

  if (!p) {
    fprintf(stderr, "platform_create: out of memory\n");
    return NULL;
  }

  signal(SIGINT, handle_sigint);
  p->cursor_hidden = true;
  fputs("\033[2J\033[?25l", stdout);
  return p;
}

void platform_destroy(platform *p) {
  if (!p)
    return;
  if (p->cursor_hidden) {
    fputs("\033[?25h", stdout);
  }
  fflush(stdout);
  free(p);
}

void platform_present(platform *p, const uint32_t *videobuffer, int w, int h) {
  (void)p;
  fputs("\033[H", stdout);
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col++) {
      putchar(videobuffer[row * w + col] ? '#' : ' ');
    }
    putchar('\n');
  }
  fflush(stdout);
}

bool platform_poll(platform *p, uint8_t keypad[PLATFORM_NUM_KEYS]) {
  (void)p;
  memset(keypad, 0, PLATFORM_NUM_KEYS);
  return !want_quit;
}

void platform_beep(platform *p, bool on) {
  (void)p;
  (void)on;
  // noop
}

uint64_t platform_now_ns(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

void platform_sleep_ns(uint64_t ns) {
  struct timespec ts = {(time_t)(ns / 1000000000ULL),
                        (long)(ns % 1000000000ULL)};
  nanosleep(&ts, NULL);
}

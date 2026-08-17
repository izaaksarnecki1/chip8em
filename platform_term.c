#include "platform.h"
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct platform {
  bool cursor_hidden;
};

// Set to 0 by the SIGINT handler so ctrl C breaks out of the loop
// without volatile, this breaks at O2 and is compiled to a noop
static volatile sig_atomic_t want_quit = 0;

static void handle_sigint(int sig) {
  (void)sig;
  want_quit = 1;
}

platform *platform_create(const char *title, int scale) {
  (void)title;
  (void)scale;
  // clears screen and hides cursor. only needed for temrinal
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

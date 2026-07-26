#include "platform.h"
#include <stdlib.h>

struct platform {};

platform *platform_create(const char *title, int scale) {
  (void)title;
  (void)scale;

  platform *p = malloc(sizeof(*p));
  return p;
}

void platform_destroy(platform *p) { free(p); }

void platform_present(platform *p, const uint32_t *videobuffer, int w, int h) {}

bool platform_poll(platform *p, uint8_t keypad[PLATFORM_NUM_KEYS]) { return 1; }

void platform_beep(platform *p, bool on) {
  // noop
}

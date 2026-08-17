#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>

#define PLATFORM_NUM_KEYS 16

typedef struct platform platform;

platform *platform_create(const char *title, int scale);
void platform_destroy(platform *p);
void platform_present(platform *p, const uint32_t *videobuffer, int w, int h);
bool platform_poll(platform *p, uint8_t keypad[PLATFORM_NUM_KEYS]);
void platform_beep(platform *p, bool on);

uint64_t platform_now_ns(void);
void platform_sleep_ns(uint64_t ns);

#endif // PLATFORM_H

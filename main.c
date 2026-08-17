#include "chip8.h"
#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CYCLES_PER_FRAME 16
#define FRAME_NS (1000000000L / 60)

static long now_ns(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int main(int argc, char *argv[]) {
  const char *filename;

  if (argc < 2) {
    filename = "roms/IBM_logo.ch8";
    printf("No file provided. Default: %s\n", filename);
  } else {
    filename = argv[1];
    printf("File selected: %s\n", filename);
  }

  chip8 chip;

  chip8_init(&chip);
  chip8_load_rom(&chip, filename);

  platform *p = platform_create("chip8", 1);
  if (!p) {
    fprintf(stderr, "Couldn't create platform. Exiting");
    return 1;
  }

  while (platform_poll(p, chip.keypad)) {
    long frame_start = now_ns();

    for (int i = 0; i < CYCLES_PER_FRAME; i++)
      chip8_cycle(&chip);

    if (chip.delay_timer > 0)
      chip.delay_timer--;
    if (chip.sound_timer > 0)
      chip.sound_timer--;

    platform_present(p, chip.videobuffer, VIDEO_WIDTH, VIDEO_HEIGHT);
    platform_beep(p, chip.sound_timer > 0);

    long remaining = FRAME_NS - (now_ns() - frame_start);
    if (remaining > 0) {
      struct timespec ts = {remaining / 1000000000L, remaining % 1000000000L};
      nanosleep(&ts, NULL);
    }
  }
  platform_destroy(p);

  return 0;
}

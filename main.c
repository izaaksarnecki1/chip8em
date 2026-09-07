#include "chip8.h"
#include "platform.h"
#include <stdint.h>
#include <stdio.h>

#define CYCLES_PER_FRAME 16
#define FRAME_NS (1000000000ULL / 60)

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

  platform *p = platform_create("chip8", 10);
  if (!p) {
    fprintf(stderr, "Couldn't create platform. Exiting");
    return 1;
  }

  while (platform_poll(p, chip.keypad)) {
    uint64_t frame_start = platform_now_ns();

    for (int i = 0; i < CYCLES_PER_FRAME; i++)
      chip8_cycle(&chip);

    if (chip.delay_timer > 0)
      chip.delay_timer--;
    if (chip.sound_timer > 0)
      chip.sound_timer--;

    platform_present(p, chip.videobuffer, VIDEO_WIDTH, VIDEO_HEIGHT);
    platform_beep(p, chip.sound_timer > 0);

    uint64_t elapsed = platform_now_ns() - frame_start;
    if (elapsed < FRAME_NS)
      platform_sleep_ns(FRAME_NS - elapsed);
  }
  platform_destroy(p);

  return 0;
}

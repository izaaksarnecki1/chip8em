#include "chip8.h"
#include "platform.h"
#include <stdint.h>
#include <stdio.h>

#define CPU_HZ 700
#define CPU_PERIOD_NS (1000000000ULL / CPU_HZ)
#define TIMER_PERIOD_NS (1000000000ULL / 60)
#define FRAME_PERIOD_NS (1000000000ULL / 60)

#define MAX_DELTA_NS 100000000ULL
#define MAX_CATCHUP_CYCLES 64
#define YIELD_NS 500000ULL

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
  if (chip8_load_rom(&chip, filename) < 0)
    return 1;

  platform *p = platform_create("chip8", 10);
  if (!p) {
    fprintf(stderr, "Couldn't create platform. Exiting");
    return 1;
  }

  uint64_t prev = platform_now_ns();
  uint64_t cpu_acc = 0, timer_acc = 0, frame_acc = 0;
  bool running = true;

  while (running && platform_poll(p, chip.keypad)) {
    uint64_t now = platform_now_ns();
    uint64_t delta = now - prev;
    prev = now;

    if (delta > MAX_DELTA_NS)
      delta = MAX_DELTA_NS;

    cpu_acc += delta;
    timer_acc += delta;
    frame_acc += delta;

    int cycles = 0;
    while (cpu_acc >= CPU_PERIOD_NS && cycles < MAX_CATCHUP_CYCLES) {
      if (chip8_cycle(&chip) < 0) {
        running = false;
        break;
      }
      cpu_acc -= CPU_PERIOD_NS;
      cycles++;
    }
    if (cpu_acc >= CPU_PERIOD_NS)
      cpu_acc %= CPU_PERIOD_NS;

    while (timer_acc >= TIMER_PERIOD_NS) {
      if (chip.delay_timer > 0)
        chip.delay_timer--;
      if (chip.sound_timer > 0)
        chip.sound_timer--;
      timer_acc -= TIMER_PERIOD_NS;
    }

    if (frame_acc >= FRAME_PERIOD_NS) {
      platform_present(p, chip.videobuffer, VIDEO_WIDTH, VIDEO_HEIGHT);
      platform_beep(p, chip.sound_timer > 0);
      frame_acc %= FRAME_PERIOD_NS;
    }

    platform_sleep_ns(YIELD_NS);
  }
  platform_destroy(p);

  return 0;
}

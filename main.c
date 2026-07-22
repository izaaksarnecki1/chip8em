#include "chip8.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CYCLES_PER_FRAME 16
#define FRAME_NS (1000000000L / 60)

// Set to 0 by the SIGINT handler so ctrl C breaks out of the loop
// without volatile, this breaks at O2 and is compiled to a noop
static volatile sig_atomic_t run = 1;

static void handle_sigint(int sig) {
  (void)sig;
  run = 0;
}

void clear_screen(void) {
  // this ansi code moves the cursor top left of the terminal (?????)
  // https://stackoverflow.com/questions/55672661/what-does-printf-033h-033j-do-in-c
  fputs("\033[H", stdout);
}

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

  // clears screen and hides cursor. only needed for temrinal
  fputs("\033[2J\033[?25l", stdout);

  signal(SIGINT, handle_sigint);

  while (run) {
    long frame_start = now_ns();

    for (int i = 0; i < CYCLES_PER_FRAME; i++)
      chip8_cycle(&chip);

    if (chip.delay_timer > 0)
      chip.delay_timer--;
    if (chip.sound_timer > 0)
      chip.sound_timer--;

    clear_screen();
    chip8_dump_display(&chip);
    fflush(stdout);

    long remaining = FRAME_NS - (now_ns() - frame_start);
    if (remaining > 0) {
      struct timespec ts = {remaining / 1000000000L, remaining % 1000000000L};
      nanosleep(&ts, NULL);
    }
  }

  // return the cursor with yet another magical ansi escape code
  fputs("\033[?25h", stdout);
  return 0;
}

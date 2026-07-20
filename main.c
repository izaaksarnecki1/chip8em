#include "chip8.h"
#include <stdio.h>

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
  for (int i = 0; i < 100; i++) {
    chip8_cycle(&chip);
  }
  chip8_dump_display(&chip);

  printf("Hello, World!\n");
  return 0;
}

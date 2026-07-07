#include "chip8.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  const char *filename;

  if (argc < 2) {
    filename = "Airplane.ch8";
    printf("No file provided. Default: Airplane.ch8\n");
  } else {
    filename = argv[1];
    printf("File selected: %s\n", filename);
  }

  chip8 chip;
  chip8_init(&chip);
  chip8_load_rom(&chip, filename);

  printf("Hello, World!\n");
  return 0;
}

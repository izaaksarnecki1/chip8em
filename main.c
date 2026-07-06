#include "chip8.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  chip8 chip;
  chip8_init(&chip);

  printf("Hello, World!\n");
  return 0;
}

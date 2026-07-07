#include "chip8.h"
#include <exception>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t fontset[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static const int FONTSET_SIZE = sizeof(fontset);

static const unsigned int FONTSET_MEM_START = 0x050;
static const unsigned int START_ADDR = 0x200;

void chip8_init(chip8 *c) {
  memset(c, 0, sizeof(*c)); // 0 out mem region before doing anything
                            // to clear the region and avoid accessing garbage

  for (int i = 0; i < FONTSET_SIZE; i++) {
    // font is usually loaded at 0x050, so we do that
    c->memory[FONTSET_MEM_START + i] = fontset[i];
  }

  // initially, program counter is at the start START_ADDR
  // when ROM is loaded, the first op will live at START_ADDR
  c->pc = START_ADDR;
}

int chip8_load_rom(chip8 *c, const char *filename) {
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Failed opening file");
    return 1;
  }

  for (int i = 0; i < ...; i++) {
    c->memory[START_ADDR + i] = file[i];
  }

  fclose(file);
}

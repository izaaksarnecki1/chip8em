#include "chip8.h"
#include <assert.h>
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
  // Open the file, point *file to the start of the file.
  // ROMs are binary files, so we rb them.
  FILE *file;
  file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Failed opening file: %s", filename);
    return -1;
  }

  size_t capacity = sizeof(c->memory) - START_ADDR;
  // read the rom file into memory of the chip.
  size_t bytes_read = fread(c->memory + START_ADDR, 1, capacity, file);

  fclose(file);

  if (bytes_read == 0) {
    fprintf(stderr, "Provided rom is empty. Read 0 bytes.\n");
    return -1;
  }

  return bytes_read;
}

int chip8_cycle(chip8 *c) {
  // fetch
  c->opcode = (c->memory[c->pc] << 8) | c->memory[c->pc + 1];
  c->pc += 2;

  // decode
  uint16_t nnn = c->opcode & 0x0FFF;     // lowest 12 bits
  uint8_t n = c->opcode & 0x000F;        // lowest byte, 0th-3rd bits
  uint8_t x = (c->opcode & 0x0F00) >> 8; // second lowest byte, 4th-7th bits
  uint8_t y = (c->opcode & 0x00F0) >> 4; // third lowest byte, 8th-11th bits
  uint8_t kk = c->opcode & 0x00FF;       // lowest 8 bits

  // execute
  switch ((c->opcode & 0xF000) >> 12) {
  case 0:
    OP_00E0(c);
    break;
  case 1:
    OP_1NNN(c, nnn);
    break;
  case 6:
    OP_6XNN(c, x, kk);
    break;
  case 7:
    OP_7XNN(c, x, kk);
    break;
  case 0xA:
    OP_ANNN(c, nnn);
    break;
  case 0xD:
    OP_DXYN(c, x, y, n);
  }

  // printf("opcode=0x%04X  nnn=0x%03X  n=0x%X  x=0x%X  y=0x%X  kk=0x%02X\n",
  //        c->opcode, nnn, n, x, y, kk);
  return 0;
}

void chip8_dump_display(const chip8 *c) {
  for (int row = 0; row < VIDEO_HEIGHT; row++) {
    for (int col = 0; col < VIDEO_WIDTH; col++) {
      putchar(c->videobuffer[row * VIDEO_WIDTH + col] ? '#' : ' ');
    }
    putchar('\n');
  }
}

void OP_00E0(chip8 *c) {
  // clears display
  memset(c->videobuffer, 0, VIDEO_WIDTH * VIDEO_HEIGHT * sizeof(uint32_t));
}

void OP_1NNN(chip8 *c, uint16_t nnn) {
  // jumps to address NNN
  c->pc = nnn;
}

void OP_6XNN(chip8 *c, uint8_t x, uint8_t kk) {
  // sets register vx to nn
  c->registers[x] = kk;
}

void OP_7XNN(chip8 *c, uint8_t x, uint8_t kk) {
  // add nn to vx
  // this insturction doesnt care about overflow, by design
  c->registers[x] += kk;
}

void OP_ANNN(chip8 *c, uint16_t nnn) {
  // sets index register to NNN
  c->index_reg = nnn;
}

void OP_DXYN(chip8 *c, uint8_t x, uint8_t y, uint8_t n) {
  // draws an N pixel tall sprite at coords at VX and VY
  uint8_t x_coord = c->registers[x] % VIDEO_WIDTH;
  uint8_t y_coord = c->registers[y] % VIDEO_HEIGHT;

  c->registers[0xF] = 0;
  for (uint8_t row = 0; row < n; row++) {
    uint8_t sprite_data = c->memory[c->index_reg + row];

    for (uint8_t col = 0; col < 8; col++) {
      uint8_t sprite_pixel = sprite_data & (0x80 >> col);
      uint32_t *screen_pixel =
          &c->videobuffer[(y_coord + row) * VIDEO_WIDTH + (x_coord + col)];

      if (sprite_pixel) {
        if (*screen_pixel == 0xFFFFFFFF) {
          // both on, set VF to 1
          c->registers[0xF] = 1;
        }
        *screen_pixel ^= sprite_pixel;
      }
    }
  }
}

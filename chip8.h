#ifndef CHIP8_H
#define CHIP8_H

#define NUM_REGISTERS 16
#define MEM_SIZE 4096
#define STACK_SIZE 16
#define NUM_INPUT_KEYS 16
#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

#include <stdint.h>

typedef struct {
  uint8_t registers[NUM_REGISTERS];
  uint8_t memory[MEM_SIZE];
  uint16_t index_reg;
  uint16_t pc;
  uint16_t stack[STACK_SIZE];
  uint8_t sp;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint8_t keypad[NUM_INPUT_KEYS];
  uint16_t opcode;
  uint32_t videobuffer[VIDEO_WIDTH * VIDEO_HEIGHT]; // use 32bit for sdl
                                                    // compatability later on
} chip8;

void chip8_init(chip8 *c);

int chip8_load_rom(chip8 *c, const char *filename);

#endif // CHIP8_H

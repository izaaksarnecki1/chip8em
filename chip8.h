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

int chip8_cycle(chip8 *c);

void chip8_dump_display(const chip8 *c);

// ------- OPS -------
void OP_00E0(chip8 *c);

void OP_1NNN(chip8 *c, uint16_t nnn);

void OP_6XNN(chip8 *c, uint8_t x, uint8_t kk);

void OP_7XNN(chip8 *c, uint8_t x, uint8_t kk);

void OP_ANNN(chip8 *c, uint16_t nnn);

void OP_DXYN(chip8 *c, uint8_t x, uint8_t y, uint8_t n);

#endif // CHIP8_H

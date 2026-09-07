# chip8em

## Build

Build with `make`

## Instructions

Currently, only a select few instructions are implemented. However, there is enough instructions in place to run IBM_logo.ch8. 
The available instructions are:

- `00E0` which clears the display.

- `1NNN` which jumps to adress `NNN`.

- `6XNN` which sets the register `VX` to hold the value `NN`.

- `7XNN` which adds `NN` to value in `VX`. For some reason, this operation does not care about overflow.

- `ANNN` which sets the index register to `NNN`

- `DXYN` which draws a `N` pixel tall sprite to coordinates found in `VX` and `VY`

## Example output

A simple display is built with SDL2. This example is running IBM_log.ch8.

<img width="647" height="350" alt="image" src="https://github.com/user-attachments/assets/9e2fc19c-11a3-4709-914c-5af9e18d2417" />

## Future work
- [x] **Cycle Loop** - impl an actual cycle loop in `main`\
- [x] **SDL Window** - get an acutal window up and running, init a renderer, impl a basic event loop
- [x] **Render videobuffer** - display videobuffer in each frame
- [ ] **Decouple timing** - Seperate  CPU speed, 60hz timers and 60hz display
- [ ] **Input** - map keyboard buttons to keypad array and connect them to sdl events.
- [ ] **Implement remaining opcodes**
- [ ] **Timers and sound** - decreement delay and sound timers, and a sound














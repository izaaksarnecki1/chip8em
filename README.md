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

Currently, no display is wired up, so the only way to see the output is to print it in the terminal. So far, running the IBM_logo ROM outputs:

<img width="806" height="615" alt="image" src="https://github.com/user-attachments/assets/715aff44-4a1b-4131-9f94-5d17cbf75639" />

## Future work
1. **Cycle Loop** - impl an actual cycle loop in `main`
2. **SDL Window** - get an acutal window up and running, init a renderer, impl a basic event loop
3. **Render videobuffer** - display videobuffer in each frame
4. **Decouple timing** - Seperate  CPU speed, 60hz timers and 60hz display
5. **Input** - map keyboard buttons to keypad array and connect them to sdl events.
6. **Implement remaining opcodes**
7. **Timers and sound** - decreement delay and sound timers, and a sound














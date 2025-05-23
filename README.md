# CHIP-8OR

A simple CHIP-8 emulator/interpreter written in C++. This emulator allows you to play classic CHIP-8 games on modern hardware.

[Video demo](https://files.catbox.moe/40sic1.mp4)

## How to Run

### Basic Usage

1. Place the emulator executable and ROM files in the same directory
2. Run the emulator using one of the following methods:

#### Method 1: Default Settings
`chip8 filename.ch8`
This runs the specified ROM with default scale and cycles per frame.

#### Method 2: Custom Scale
`chip8 <scale> filename.ch8`
This runs the specified ROM with a custom scale factor and default cycles per frame.

#### Method 3: Custom Scale and Tickrate
`chip8 <scale> <cycles/frame> filename.ch8`
This runs the specified ROM with custom scale factor and custom cycles per frame.

### Parameters

- `scale`: Display scaling factor (integer, recommended values: 5-20)
- `cycles_per_frame`: Number of CHIP-8 instructions executed per frame (integer,    recommended values: 5-15)
  - Higher values make games run faster
  - Lower values make games run slower
  - Effective instruction rate = cycles_per_frame × 60 Hz
- `filename.ch8`: Path to the CHIP-8 ROM file

## Controls

The CHIP-8 system uses a 16-key hexadecimal keypad. These keys are mapped to your keyboard as follows:

| CHIP8 Key | Keyboard Key |
|------------|--------------|
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
| C | 4 |
| 4 | Q |
| 5 | W |
| 6 | E |
| D | R |
| 7 | A |
| 8 | S |
| 9 | D |
| E | F |
| A | Z |
| 0 | X |
| B | C |
| F | V |

### Special Keys

- `ESC`: Exit the emulator
- `J`: Reset the current ROM
- `Space`: Pause/Resume emulation

## How to Build

To build this project, follow these steps:

1. **Make sure you have the following files in the project folder**:
   - `raylib.h`
   - `libraylib.a`

   These files are required from a working [raylib](https://github.com/raysan5/raylib) build.

2. **Run the Python build script**:
   ```bash
   build.py
   ```
   this will build the executable file 
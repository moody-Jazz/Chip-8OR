#include "chip8.hpp"
#include "interface.hpp"

int main(int argc, char* argv[]) {
    int cyclesPerFrame = 10;
    
    Chip8 chip8;
    Interface interface;
    std::string rom;
    
    // the function below returns 1 if an error occured
    if(interface.handleCmdArgs(argc, argv, cyclesPerFrame, rom)) return 1;
    
    chip8.loadRom(rom);
    
    InitWindow(SCREEN_WIDTH * interface.scale, SCREEN_HEIGHT * interface.scale, "CHIP-8 Emulator");
    SetTargetFPS(60);
    InitAudioDevice();

    Sound beep = LoadSound("beep.wav");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        interface.inputHandler(chip8.keypad);

        if(interface.restart){
            chip8.restartProgram();
            interface.restart = false;
        }

        if(!interface.pause)
            for (int i = 0; i < cyclesPerFrame; ++i) 
                chip8.cycle();

        if(chip8.soundTimer == 1)
            PlaySound(beep);

        chip8.decrementTimers();
        interface.updateFrame(chip8.displayBuffer, SCREEN_HEIGHT, SCREEN_WIDTH);

        EndDrawing();
    }
    
    UnloadSound(beep);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
#include "interface.hpp"

void Interface::setScale(size_t scale){
    this->scale = scale;
}

void Interface::updateFrame(uint32_t *display, size_t height, size_t width){
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (display[y * width + x]) {
                DrawRectangle(x * scale, y * scale, scale, scale, GREEN);
            }
        }
    }
}

void Interface::inputHandler(uint8_t *keypad){
    /* ====================
     * keyboard key mapping
     * ====================
     *
     *  1 2 3 C  -> 1 2 3 4
     *  4 5 6 D  -> Q W E R
     *  7 8 9 E  -> A S D F
     *  A 0 B F  -> Z X C V
    */
    if(IsKeyPressed(KEY_SPACE))
        pause = !pause;

    if(IsKeyPressed(KEY_J))
        restart = !restart;

    keypad[0x0] = IsKeyDown(KEY_X);
    keypad[0x1] = IsKeyDown(KEY_ONE);
    keypad[0x2] = IsKeyDown(KEY_TWO);
    keypad[0x3] = IsKeyDown(KEY_THREE);
    keypad[0x4] = IsKeyDown(KEY_Q);
    keypad[0x5] = IsKeyDown(KEY_W);
    keypad[0x6] = IsKeyDown(KEY_E);
    keypad[0x7] = IsKeyDown(KEY_A);
    keypad[0x8] = IsKeyDown(KEY_S);
    keypad[0x9] = IsKeyDown(KEY_D);
    keypad[0xA] = IsKeyDown(KEY_Z);
    keypad[0xB] = IsKeyDown(KEY_C);
    keypad[0xC] = IsKeyDown(KEY_FOUR);
    keypad[0xD] = IsKeyDown(KEY_R);
    keypad[0xE] = IsKeyDown(KEY_F);
    keypad[0xF] = IsKeyDown(KEY_V);   
}

int Interface::handleCmdArgs(int argc, char* argv[], int cyclesPerFrame, std::string& rom){
    // if one argument is passed then it must be ROM file name
    // if two arguments are passed then they must be <scale, ROM-name> (scale is size of one pixel)
    // if three arguments are passed then they must be <scale, tickrate, ROM-name> (tickrate is amount of cycles per frame)
    
    if(argc == 2){
        rom = argv[1];
        return 0;
    }
    else if(argc == 1){
        std::cerr<<"Expected atleast one argument\n";
        return 1;
    }

    try{
        if(argc == 3){
            setScale(std::stoi(argv[1]));
            rom = argv[2];
            return 0;
        }
        else if(argc == 4){
            setScale(std::stoi(argv[1]));
            cyclesPerFrame = std::stoi(argv[2]);
            rom = argv[3];
            return 0;
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error: first two parameters must be a number\n";
        return 1;
    }
    std::cerr <<"Too many arguments\n";
    return 1;
}
#pragma once

#include "string"
#include "cstdint"
#include "raylib.h"
#include "iostream"

class Interface{
public:
    bool restart = false, pause = false;
    size_t scale = 25;
    void setScale(size_t scale);
    void updateFrame(uint32_t *display, size_t height, size_t width);
    void inputHandler(uint8_t *keypad);
    int handleCmdArgs(int argc, char* argv[], int cyclesPerFrame, std::string& rom);
};
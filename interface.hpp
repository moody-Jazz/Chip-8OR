#pragma once

#include "cstdint"
#include "raylib.h"

constexpr size_t SCALE = 25;

class Interface{
public:
    void updateFrame(uint32_t *display, size_t height, size_t width);
    void inputHandler();
};
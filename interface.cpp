#include "interface.hpp"

void Interface::updateFrame(uint32_t *display, size_t height, size_t width){
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (display[y * width + x]) {
                DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, GREEN);
            }
        }
    }
}

void Interface::inputHandler(){
    /* ====================
     * keyboard key mapping
     * ====================
     *
     *  1 2 3 C  -> 1 2 3 4
     *  4 5 6 D  -> Q W E R
     *  7 8 9 E  -> A S D F
     *  A 0 B F  -> Z X C V
    */
    
}
#include "chip8.hpp"

int main()
{
    Chip8 cpu;
    cpu.loadRom("IBM.ch8");
    return 0;
}
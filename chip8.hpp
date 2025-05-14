#pragma once

#include "iostream"
#include "cstdint"
#include "string"
#include "fstream"
#include "vector"
#include "iomanip"

#define STARTING_ADDRESS 0x200

class Chip8{
public:
    Chip8();
    ~Chip8();
    void loadRom(const std::string fileName);
    
private:
    uint8_t  regV_[16];
    uint16_t indexReg_;
    uint16_t prgrmCntr_;
    uint8_t  stkPtr_;

    uint8_t memory_[4096];
};
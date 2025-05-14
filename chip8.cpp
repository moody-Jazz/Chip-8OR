#include "chip8.hpp"

Chip8::Chip8() = default;
Chip8::~Chip8() = default;

void Chip8::loadRom(const std::string filename){
    std::ifstream file;
    file.open(filename, std::ios::binary);

    if (!file.is_open()){
        std::cerr << "couldn't open the ROM file\n";
        return;
    }

    file.seekg(0, file.end);
    size_t size = file.tellg();
    file.seekg(0, file.beg);

    std::vector<char> buffer(size);

    if (!file.read(buffer.data(), size)) {
        std::cerr << "Failed to read the ROM file data\n";
        return;
    }

    for (int i{}; i < size; i++)
        memory_[STARTING_ADDRESS + i] = buffer[i];
        
    file.close();
}
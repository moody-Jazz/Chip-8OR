#include "chip8.hpp"

Chip8::Chip8(){
    prgrmCntr_ = STARTING_ADDRESS;
    loadFontset();
} 

Chip8::~Chip8() = default;

const uint8_t Chip8::FONTSET[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6 
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8 
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::loadFontset(){
    for(int i{}; i<80; i++)
        memory_[FONTSET_STARTING + i] = FONTSET[i];
}

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

void Chip8::CLS(){
    
}

void Chip8::RET(){
}

void Chip8::SYS_addr(){
}

void Chip8::JP_addr(){
}

void Chip8::CALL_addr(){
}

void Chip8::SE_Vx(){
}

void Chip8::SNE_Vx(){
}

void Chip8::SE_Vx_Vy(){
}

void Chip8::LD_Vx(){
}

void Chip8::ADD_Vx(){
}

void Chip8::LD_Vx_Vy(){
}

void Chip8::OR_Vx_Vy(){
}

void Chip8::AND_Vx_Vy(){
}

void Chip8::XOR_Vx_Vy(){
}

void Chip8::ADD_Vx_Vy(){
}

void Chip8::SUB_Vx_Vy(){
}

void Chip8::SHR_Vx(){
}

void Chip8::SUBN_Vx_Vy(){
}

void Chip8::SHL_Vx(){
}

void Chip8::SNE_Vx_Vy(){
}

void Chip8::LD_I(){
}

void Chip8::JP_V0(){
}

void Chip8::RND_Vx(){
}

void Chip8::DRW_Vx_Vy(){
}

void Chip8::SKP_Vx(){
}

void Chip8::SKNP_Vx(){
}

void Chip8::LD_Vx_DT(){
}

void Chip8::LD_Vx_K(){
}

void Chip8::LD_DT_Vx(){
}

void Chip8::LD_ST_Vx(){
}

void Chip8::ADD_I_Vx(){
}

void Chip8::LD_F_Vx(){
}

void Chip8::LD_B_Vx(){
}

void Chip8::LD_I_Vx(){
}

void Chip8::LD_Vx_I(){
}

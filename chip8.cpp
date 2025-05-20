#include "chip8.hpp"

Chip8::Chip8(){
    prgrmCntr_ = STARTING_ADDRESS;

    // SYS_ADDR() is the default instruction which does nothing so that we do nothing for illegal instructions
    for(int i{}; i<TABLE1_SIZE; i++){
        table1[i] = &SYS_addr;
        table2[i] = &SYS_addr;
    }
    for(int i{}; i<TABLE3_SIZE; i++)
        table3[i] = &SYS_addr;

    table1[0x1] = &JP_addr;
    table1[0x2] = &CALL_addr;
    table1[0x3] = &SE_Vx;
    table1[0x4] = &SNE_Vx;
    table1[0x5] = &SE_Vx_Vy;
    table1[0x6] = &LD_Vx;
    table1[0x7] = &ADD_Vx;
    table1[0x9] = &SNE_Vx_Vy;
    table1[0xA] = &LD_I;
    table1[0xB] = &JP_V0;
    table1[0xC] = &RND_Vx;
    table1[0xD] = &DRW_Vx_Vy;

    table2[0x0] = &LD_Vx_Vy;
    table2[0x1] = &OR_Vx_Vy;
    table2[0x2] = &AND_Vx_Vy;
    table2[0x3] = &XOR_Vx_Vy;
    table2[0x4] = &ADD_Vx_Vy;
    table2[0x5] = &SUB_Vx_Vy;
    table2[0x6] = &SHR_Vx;
    table2[0x7] = &SUBN_Vx_Vy;
    table2[0xE] = SHL_Vx;

    /*
     * for unique indexing of each instruction in table3 the last two nibbles are taken from left
     * example: for instruction $Ex9E, 9 and E are the nibbles which make unique pair 
     * suppose 9 is x and E is y then the formula (x *  TABLE3_OFFSET + y) is used to find the 
     * unique index for the instruction $Ex9E 
     * (TABLE3_OFFSET = 3 is the lowest number which gives unique index for all instructions in table3)
    */
    table3[42] = &CLS;
    table3[56] = &RET;
    table3[31] = &SKNP_Vx;
    table3[41] = &SKP_Vx;
    table3[7]  = &LD_Vx_DT;
    table3[10] = &LD_Vx_K;
    table3[8]  = &LD_DT_Vx;
    table3[11] = &LD_ST_Vx;
    table3[17] = &ADD_I_Vx;
    table3[15] = &LD_F_Vx;
    table3[12] = &LD_B_Vx;
    table3[20] = &LD_I_Vx;
    table3[23] = &LD_Vx_I;

    loadFontset();
} 

Chip8::~Chip8() = default;

void Chip8::cycle(){
    opcode_ = (memory_[prgrmCntr_] << 8) | memory_[prgrmCntr_ + 1];
    prgrmCntr_ += 2;

    // call the appropriate instruction after extracting the type of instruction opcode contains
    uint8_t instructionType = (opcode_ & 0xF000) >> 12;
    
    if(instructionType >= 1 && instructionType <= 0xD && instructionType != 8)
        (this->*table1[instructionType])();

    else if(instructionType == 8)
        (this->*table2[opcode_ & 0x000F])();

    else{
        uint8_t x = (opcode_ & 0x00F0) >> 4;
        uint8_t y = opcode_ & 0x000F;
        (this->*table3[x * TABLE3_OFFSET + y])();
    }

    if (delayTimer_ > 0) delayTimer_--;
    if (soundTimer_ > 0) soundTimer_--;
}

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

uint8_t Chip8::getRandByte(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    uint8_t randByte = static_cast<uint8_t>(dist(gen));
    return randByte;
}

void Chip8::CLS(){
    for(int i{}; i < 64 * 32; i++)
        displayBuffer[i] = 0;
}

void Chip8::RET(){
    prgrmCntr_ = stack_[--stkPtr_];
}

void Chip8::SYS_addr(){
}

void Chip8::JP_addr(){
    prgrmCntr_ = (opcode_ & 0x0FFF);
}

void Chip8::CALL_addr(){
    stack_[stkPtr_++] = prgrmCntr_;
    prgrmCntr_ = (opcode_ & 0xFFF);
}

void Chip8::SE_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t kk = (opcode_ & 0x00FF);

    if(regV_[x] == kk)
        prgrmCntr_ += 2;
}

void Chip8::SNE_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t kk = (opcode_ & 0x00FF);

    if(regV_[x] != kk)
        prgrmCntr_ += 2;
}

void Chip8::SE_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    if(regV_[x] == regV_[y])
        prgrmCntr_ += 2;
}

void Chip8::LD_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t kk = (opcode_ & 0x00FF);

    regV_[x] = kk;
}

void Chip8::ADD_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t kk = (opcode_ & 0x00FF);

    regV_[x] += kk;
}

void Chip8::LD_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    regV_[x] = regV_[y];
}

void Chip8::OR_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    regV_[x] = regV_[x] | regV_[y];
}

void Chip8::AND_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    regV_[x] = regV_[x] & regV_[y];
}

void Chip8::XOR_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    regV_[x] = regV_[x] ^ regV_[y];
}

void Chip8::ADD_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    uint16_t sum = regV_[x] + regV_[y];
    
    if(sum > 225)
        regV_[VF] = 1;
    else    
        regV_[VF] = 0;
    
    regV_[x] = sum & 0xFF;
}

void Chip8::SUB_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    if(regV_[x] > regV_[y])
        regV_[VF] = 1;
    else 
        regV_[VF] = 0;

    regV_[x] -= regV_[y];
}

void Chip8::SHR_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    if(regV_[x] & 1U)
        regV_[VF] = 1;
    else    
        regV_[VF] = 0;
         
    regV_[x] >>= 1;
}

void Chip8::SUBN_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    if(regV_[y] > regV_[x])
        regV_[VF] = 1;
    else    
        regV_[VF] = 0;

    regV_[x] = regV_[y] - regV_[x];
}

void Chip8::SHL_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    if(regV_[x] & (1U<<7))
        regV_[VF] = 1;
    else
        regV_[VF] = 0;

    regV_[x] <<= 1;
}

void Chip8::SNE_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;

    if(regV_[x] != regV_[y])
        prgrmCntr_ += 2;
}

void Chip8::LD_I(){
    indexReg_ = opcode_ & 0x0FFF;
}

void Chip8::JP_V0(){
    uint16_t jmpLocation = opcode_ & 0xFFF;
    jmpLocation += regV_[0];

    prgrmCntr_ = jmpLocation;
}

void Chip8::RND_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t kk = (opcode_ & 0x00FF);

    regV_[x] = getRandByte() & kk;
}

void Chip8::DRW_Vx_Vy(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t y = (opcode_ & 0x00F0) >> 4;
    int n = opcode_ & 0x000F;

    uint8_t xcord = regV_[x] % SCREEN_WIDTH,
            ycord = regV_[y] % SCREEN_HEIGHT;
    uint8_t spriteByte, spriteBit;

    regV_[VF] = 0;

    for(int i{}; i<n; i++){
        spriteByte = memory_[indexReg_ + i];
        for(int j{}; j<8; j++){
            spriteBit = spriteByte & (0x80 >> j);
            uint32_t* screenPixel = &displayBuffer[(ycord + i) * SCREEN_WIDTH + (xcord + j)];
            
            if(spriteBit){
                if(*screenPixel == 0xFFFFFFFF)
                    regV_[VF] = 1;
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::SKP_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
	uint8_t key = regV_[x];

	if (keypad[key])
		prgrmCntr_ += 2;
}

void Chip8::SKNP_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
	uint8_t key = regV_[x];

	if (!keypad[key])
		prgrmCntr_ += 2;
}

void Chip8::LD_Vx_DT(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    regV_[x] = delayTimer_;
}

void Chip8::LD_Vx_K(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    if (keypad[0])
		regV_[x] = 0;
	else if (keypad[1])
		regV_[x] = 1;
	else if (keypad[2])
		regV_[x] = 2;
	else if (keypad[3])
		regV_[x] = 3;
	else if (keypad[4])
		regV_[x] = 4;
	else if (keypad[5])
		regV_[x] = 5;
	else if (keypad[6])
		regV_[x] = 6;
	else if (keypad[7])
		regV_[x] = 7;
	else if (keypad[8])
		regV_[x] = 8;
	else if (keypad[9])
		regV_[x] = 9;
	else if (keypad[10])
		regV_[x] = 10;
	else if (keypad[11])
		regV_[x] = 11;
	else if (keypad[12])
		regV_[x] = 12;
	else if (keypad[13])
		regV_[x] = 13;
	else if (keypad[14])
		regV_[x] = 14;
	else if (keypad[15])
		regV_[x] = 15;
	else
		prgrmCntr_ -= 2;
}

void Chip8::LD_DT_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    delayTimer_ = regV_[x];
}

void Chip8::LD_ST_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    soundTimer_ = regV_[x];
}

void Chip8::ADD_I_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    indexReg_ += regV_[x];
}

void Chip8::LD_F_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t digit = regV_[x];

    indexReg_ = FONTSET_STARTING + (5 * digit);
}

void Chip8::LD_B_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;
    uint8_t val = regV_[x];

    memory_[indexReg_] = val / 100;
    val %= 100;

    memory_[indexReg_ + 1] = val / 10;
    val %= 10;

    memory_[indexReg_ + 2] = val;
}

void Chip8::LD_I_Vx(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    for(int i{}; i <= x; i++)
        memory_[indexReg_ + i] = regV_[i];
}

void Chip8::LD_Vx_I(){
    uint8_t x = (opcode_ & 0x0F00) >> 8;

    for(int i{}; i <= x; i++)
        regV_[i] = memory_[indexReg_ + i];
}

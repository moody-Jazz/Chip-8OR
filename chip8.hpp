#pragma once

#include "iostream"
#include "cstdint"
#include "string"
#include "fstream"
#include "array"
#include "iomanip"
#include "random"

constexpr uint16_t STARTING_ADDRESS = 0x200;
constexpr uint16_t FONTSET_STARTING = 0x050;
constexpr uint8_t TABLE1_SIZE       = 16;
constexpr uint8_t TABLE2_SIZE       = 16;
constexpr uint8_t TABLE3_SIZE       = 57;
constexpr uint8_t TABLE3_OFFSET     = 3;
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

class Chip8{
public:
    Chip8();
    ~Chip8();
    void cycle();

    static const uint8_t FONTSET[80];
    uint32_t displayBuffer[64 * 32]{};
    uint8_t keypad[16]{};

    /*
     * below are the lookup tables for all the instructions
     * these arrays contain function pointers for each instruction
     * The instructions set is divided in three groups
     * table1 is group of instructions which all have unique most significant nibble
     * table2 is group of instructions which all have unique least significant nibble
     * table3 is group of instructions which all have unique pair of 0th and 1st nibble
     * 
     * table1 = $1nnn  $2nnn  $3xkk  $4xkk  $5xy0  $6xkk
                $7xkk  $9xy0  $Annn  $Bnnn  $Cxkk  $Dxyn
                as you can see the first nibble from left is unique

     * table2 = $8xy0  $8xy1  $8xy2  $8xy3 
                $8xy5  $8xy6  $8xy7  $8xyE  $8xy4
                as you can see the last nibble from left is unique

     * table3 = $00E0  $00EE  $ExA1  $Ex9E  $Fx07  $Fx0A
                $Fx1E  $Fx29  $Fx33  $Fx55  $Fx65  $Fx18  $Fx15  
                as you can see the pair of last two nibble from left are all unique
    */
    std::array<void (Chip8::*)(), TABLE1_SIZE> table1;
    std::array<void (Chip8::*)(), TABLE2_SIZE> table2;
    std::array<void (Chip8::*)(), TABLE3_SIZE> table3;
    
    enum REGISTERS{
        VA = 10,
        VB = 11,
        VC = 12,
        VD = 13,
        VE = 14,
        VF = 15
    };

    void loadFontset();
    void loadRom(const std::string fileName);
    uint8_t getRandByte();

    // instruction set

    void CLS();         // clear the display
    void RET();         // return from a subroutine
    void SYS_addr();    // jump to a machine code routine at nnn
    void JP_addr();     // jump to location nnn
    void CALL_addr();   // call subroutine at nnn
    void SE_Vx();       // skip next instruction if Vx = kk
    void SNE_Vx();      // skip next instruction if Vx != kk
    void SE_Vx_Vy();    // skip next instruction if Vx = Vy
    void LD_Vx();       // set Vx = kk
    void ADD_Vx();      // set Vx = Vx + kk
    void LD_Vx_Vy();    // set Vx = Vy
    void OR_Vx_Vy();    // set Vx = Vx OR Vy
    void AND_Vx_Vy();   // set Vx = Vx AND Vy
    void XOR_Vx_Vy();   // set Vx = Vx XOR Vy
    void ADD_Vx_Vy();   // set Vx = Vx + Vy, set VF = carry
    void SUB_Vx_Vy();   // set Vx = Vx - Vy, set VF = NOT borrow
    void SHR_Vx();      // set Vx = Vx SHR 1
    void SUBN_Vx_Vy();  // set Vx = Vy - Vx, set VF = NOT borrow
    void SHL_Vx();      // set Vx = Vx SHL 1
    void SNE_Vx_Vy();   // skip next instruciton if Vx != Vy
    void LD_I();        // set I = nnn
    void JP_V0();       // jump to location nnn + V0
    void RND_Vx();      // set Vx = random byte AND kk
    void DRW_Vx_Vy();   // display n-byte sprite starting at mem location I at (Vx, Vy), set Vf = collision
    void SKP_Vx();      // skip next instruction if key with the value of Vx is pressed
    void SKNP_Vx();     // skip next instruction if key with the value of Vx is not pressed
    void LD_Vx_DT();    // set Vx = delay timer value
    void LD_Vx_K();     // wait for a key press, store the value of the key in Vx
    void LD_DT_Vx();    // set delay timer = Vx
    void LD_ST_Vx();    // set sound timer = Vx
    void ADD_I_Vx();    // set I = I + Vx
    void LD_F_Vx();     // set I = location of sprite for digit Vx
    void LD_B_Vx();     // store BCD representation of Vx in mem location I, I+1 and I+2
    void LD_I_Vx();     // store registers V0 throgh Vx in memory, starting at location I
    void LD_Vx_I();     // Read registers V0 through Vx from memory starting at location I.
    
private:
    uint16_t opcode_{};
    uint16_t indexReg_{};
    uint16_t prgrmCntr_{};
    uint8_t  stkPtr_{};
    uint8_t delayTimer_{};
    uint8_t soundTimer_{};

    uint8_t  regV_[16]{};
    uint8_t memory_[4096]{};
    uint16_t stack_[16]{};
};
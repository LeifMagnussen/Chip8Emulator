#pragma once
#include "stdafx.h"


/* CHIP8 Memory Map:
    0x000-0x1FF - Interpreter (Contains font sets in emu)
    0x050-0x0A0 - Built in 4x5 pixel set
    0x200-0xFFF - Program ROM and work RAM
 */

class Chip8 {
    public:
        unsigned short opCode;
        std::string opCodeString;
        unsigned long gfx[64 * 32];

        // system register, memory and stack. Moved to public for use in opCodes.cpp
        unsigned char memory[4096];
        unsigned char V[16]; // Register 16, saved as flag register
        unsigned short I; // index register
        unsigned short pc; //program counter
        unsigned short stack[16]; // used for subroutine callbacks
        unsigned short sp; // stack pointer
        bool drawFlag;
        unsigned char delayTimer; // timer register (@60Hz) when above zero counts down
        unsigned char soundTimer; // 60Hz register, buzzes when timer hits zero
        bool exit; // exit flag
        unsigned char key[0xF];

        void setKeys();
        void emulateCycle();
        void initialize();
        void loadGame(const char* gameName);
        void setupGraphics();
        void setupInput();
        void drawGraphics();

         // SDL variables
        const int m_SCREEN_WIDTH = 640;
        const int m_SCREEN_HEIGHT = 320;

    private:
        unsigned char m_fontSet[80] = 
        {
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
        
};
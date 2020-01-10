#include "stdafx.h"
#include <sstream>
#include "Chip8.h"
#include "opCodes.h"

void Chip8::initialize()
{
    
    // initialize registers and memory block
    this->pc = 0x200;
    this->I = 0;
    this->sp = 0;

    // clear display
    for (int i = 0; i < (64 * 32); i++)
        this->gfx[i] = 0;
    
    // clear stack
    for (int i = 0; i < 16; i++)
        this->stack[i] = 0;

    // clear registers V0-VF
    for (int i = 0; i < 0xF; i++)
        this->V[i] = 0;

    // clear memory
    for (int i = 0; i < 4096; i++)
        this->memory[i] = 0;

    // load fontset
    for (int i = 0; i < 80; i++)
        this->memory[i] = this->m_fontSet[i];

    // reset timers
    this->soundTimer = 0;
    this->delayTimer = 0;

}


void Chip8::setupInput()
{
    return;
}

void Chip8::emulateCycle()
{
    // Fetch Opcode
    this->opCode = this->memory[this->pc] << 8 | this->memory[this->pc + 1];
    std::stringstream tmpStream;
    tmpStream << std::setfill('0') << std::setw(sizeof(short)*2) << std::hex << this->opCode;
    this->opCodeString = tmpStream.str();

    // Decode Opcode
    opCodes currOp(*this);
    currOp.executeCurrInstruction(*this);

    // Update timers and pc
    this->pc += 2;
    if (this->soundTimer > 0)
        {
            if (this->soundTimer == 1)
                printf("BEEP\n");
            --this->soundTimer;
        }
    
    if (this->delayTimer > 0)
        --this->delayTimer;
}

void Chip8::loadGame(const char* gameName)
{
    // opening file stream for the game file
    FILE * gameFile;
    gameFile = fopen(gameName, "r");

    // getting the size of the file to allocate a buffer
    fseek(gameFile, 0, SEEK_END);
    int gameSize = ftell(gameFile);
    unsigned short* buff;
    buff = (unsigned short*) malloc(gameSize);
    unsigned short currVal;

    for (int i = 0; i < gameSize; i++)
    {
        currVal = fgetc(gameFile);
        buff[i] = currVal;
    }     

    fclose(gameFile);


    // writing the memory contents of the game file
    for (int i = 0; i < gameSize; i++)
    {
        this->memory[i + 512] = buff[i];
    }
    return;        
}

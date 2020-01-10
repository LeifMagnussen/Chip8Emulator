#include "stdafx.h"
#include "opCodes.h"
#include "Chip8.h"


void opCodes::executeCurrInstruction(Chip8 myChip)
{
    opCodeMemFn a[] = { &opCodes::f00E0, &opCodes::f00EE, &opCodes::f1NNN, &opCodes::f2NNN, &opCodes::f3XNN,
                            &opCodes::f4XNN, &opCodes::f5XY0, &opCodes::f6XNN, &opCodes::f7XNN, &opCodes::f7XNN, &opCodes::f8XY0, &opCodes::f8XY1,
                            &opCodes::f8XY2, &opCodes::f8XY3, &opCodes::f8XY4, &opCodes::f8XY5, &opCodes::f8XY6, &opCodes::f8XY7, &opCodes::f8XYE,
                            &opCodes::f9XY0, &opCodes::fANNN, &opCodes::fBNNN, &opCodes::fCXNN, &opCodes::fDXYN, &opCodes::fEX9E, &opCodes::fEXA1,
                            &opCodes::fFX07, &opCodes::fFX0A, &opCodes::fFX55, &opCodes::fX15, &opCodes::fX18, &opCodes::fX1E, &opCodes::fX29,
                            &opCodes::fX33, &opCodes::fX65  
    };
    int currOpCodeIndex = this->currOpCodeFunc;
    std::invoke(a[this->currOpCodeFunc], this, myChip);
}

void opCodes::f00E0(Chip8 &myChip)
{
    // clears the screen, filling gfx with zeroes
    for (int i = 0; i < 2048; i++)
        myChip.gfx[i] = 0;

    return;
}

void opCodes::f00EE(Chip8 &myChip)
{
    // return from subroutine, copy pc from stack
    myChip.pc = myChip.stack[myChip.sp];
    myChip.sp -= 4;
    return;
}
void opCodes::f1NNN(Chip8 &myChip)
{
    // Jumps to address at NNN
    myChip.pc = myChip.opCode & 0x0FFF;
    return;
}

void opCodes::f2NNN(Chip8 &myChip)
{
    // calls subroutine at address NNN
    myChip.stack[myChip.sp] = myChip.pc;
    myChip.sp += 4;
    myChip.pc = myChip.opCode & 0x0FFF; 
    return;
}

void opCodes::f3XNN(Chip8 &myChip)
{
    // Skips the next instruction if V[X] == NN
    if (myChip.V[(myChip.opCode & 0x0F00) >> 8] == (myChip.opCode & 0x00FF))
        myChip.pc += 4;
    return;
}

void opCodes::f4XNN(Chip8 &myChip)
{
    // Skips next instruction if V[X] != NN
    if (myChip.V[(myChip.opCode & 0x0F00) >> 8] != (myChip.opCode & 0x00FF))
        myChip.pc += 4;
    return;
}

void opCodes::f5XY0(Chip8 &myChip)
{
    // skips the next instruction if V[X] == V[Y]
    if (myChip.V[(myChip.opCode & 0x0F00) >> 8] == ((myChip.opCode & 0x00F0) >> 4))
        myChip.pc += 4;
    return;
}

void opCodes::f6XNN(Chip8 &myChip)
{
    // sets V[X] == NN
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = myChip.opCode & 0x00FF;
    return;
}

void opCodes::f7XNN(Chip8 &myChip)
{
    // Adds NN to V[X] (No carry flag change)
    myChip.V[(myChip.opCode & 0x0F00) >> 8] += myChip.opCode & 0x00FF;
    return;
}

void opCodes::f8XY0(Chip8 &myChip)
{
    // Sets VX to the value of VY.
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = ( (myChip.opCode & 0x000F0) >> 4);
    return;
}

void opCodes::f8XY1(Chip8 &myChip)
{
    // Sets VX to VX or VY. (Bitwise OR operation)
    myChip.V(myChip.opCode & 0x0F00) >> 8] = ( ((myChip.opCode & 0x0F00) >> 8) | ((myChip.opCode & 0x00F0) >> 4) );
    return;
}

void opCodes::f8XY2(Chip8 &myChip)
{
    // Sets VX to VX and VY. (Bitwise AND operation)
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = ( ((myChip.opCode & 0x0F00) >> 8) & ((myChip.opCode & 0x00F0) >> 4) );
    return;
}

void opCodes::f8XY3(Chip8 &myChip)
{
    // Sets VX to VX xor VY.
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = ( ((myChip.opCode & 0x0F00) >> 8) ^ ((myChip.opCode & 0x00F0) >> 4) );
    return;
}

void opCodes::f8XY4(Chip8 &myChip)
{
    // adds the value of VY to VX (note big endian storage)
    if (myChip.V[(myChip.opCode & 0x00F0) >> 4] > (0xFF - myChip.V[(myChip.opCode & 0x0F00) >> 8]))
        myChip.V[0xF] = 1; // carry bit
    else
        myChip.V[0xF] = 0;

    myChip.V[(myChip.opCode & 0x0F00) >> 8] += myChip.V[(myChip.opCode & 0x00F0) >> 4];

    return;
}
void opCodes::f8XY5(Chip8 &myChip)
{
    // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
    if (myChip.V[(myChip.opCode & 0x00F0) >> 4] > myChip.V[(myChip.opCode & 0x0F00) >> 8])
        myChip.V[0xF] = 1; // borrow needed so set carry bit
    else
        myChip.V[0xF] = 0;
    myChip.V[(myChip.opCode & 0x0F00) >> 8] -= myChip.V[(myChip.opCode & 0x00F00) >> 4] ;
    return;
}

void opCodes::f8XY6(Chip8 &myChip)
{
    // Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
    myChip.V[0xF] = myChip.V[(myChip.opCode & 0x0F00) >> 8]; // note big endian
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = myChip.V[(myChip.opCode & 0x0F00) >> 8] >> 1;
    return;
}

void opCodes::f8XY7(Chip8 &myChip)
{
    // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
    if (myChip.V[(myChip.opCode & 0x0F00) >> 8] > myChip.V[(myChip.opCode & 0x00F0) >> 4])
        myChip.V[0xF] = 0;
    else
        myChip.V[0xF] = 1;
    return;
}

void opCodes::f8XYE(Chip8 &myChip)
{
    // Stores the most significant bit of VX in VF and then shifts VX to the left by 1.[b]
    myChip.V[0xF] = myChip.V[(myChip.opCode & 0x0F00) >> 8];
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = myChip.V[(myChip.opCode & 0x0F00) << 1];
    return;
}

void opCodes::f9XY0(Chip8 &myChip)
{
    // Skips the next instruction if VX doesn't equal VY. (Usually the next instruction is a jump to skip a code block)
    if (myChip.V[myChip.opCode & 0x0F00] != myChip.V[myChip.opCode & 0x00F0])
        myChip.pc += 4;
    return;
}

void opCodes::fANNN(Chip8 &myChip)
{
    // Sets I to the address NNN.
    myChip.I = myChip.opCode & 0x0FFF;
    return;
}

void opCodes::fBNNN(Chip8 &myChip)
{
    // Jumps to the address NNN plus V0.
    myChip.pc = (myChip.opCode & 0x0FFF) + myChip.V[0];
    return;
}

void opCodes::fCXNN(Chip8 &myChip)
{
    // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN
    unsigned short tmpRand = rand() % 256;
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = tmpRand & (myChip.opCode & 0x00FF);
    return;
}

void opCodes::fDXYN(Chip8 &myChip)
{
    // pixel drawing opcode
    unsigned short xPos = myChip.V[(myChip.opCode & 0x0F00) >> 8];
    unsigned short yPos = myChip.V[(myChip.opCode & 0x00F0) >> 4];
    unsigned short height = myChip.opCode & 0x000F;
    unsigned short pixel;

    myChip.V[0xF] = 0;
    for (int yLine = 0; yLine < height; yLine++)
    {
        pixel = myChip.memory[myChip.I + yLine];
        for (int xLine = 0; xLine < 8; xLine++)
        {
            if ((pixel & (0x80 >> xLine)) != 0)
            {
                if (myChip.gfx[xPos + xLine + (yPos + yLine) * 64] == 1)
                    myChip.V[0xF] = 1;
                myChip.gfx[xPos + xLine + (yPos + yLine) * 64] ^= 1;
            }
        }
    }
    myChip.drawFlag = true;
    return;
}

void opCodes::fEX9E(Chip8 &myChip)
{
    // Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)
    if (myChip.key[myChip.V[(myChip.opCode & 0x0F00) >> 8]] != 0)
        myChip.pc += 4;
    return;
}

void opCodes::fEXA1(Chip8 &myChip)
{
    // Skips the next instruction if the key stored in VX isn't pressed. (Usually the next instruction is a jump to skip a code block)
    if (myChip.key[myChip.V[(myChip.opCode & 0x0F00) >> 8]] == 0)
        myChip.pc += 4;
    return;
}

void opCodes::fFX07(Chip8 &myChip)
{
    // Sets VX to the value of the delay timer.
    myChip.V[(myChip.opCode & 0x0F00) >> 8] = myChip.delayTimer;
    return;
}

void opCodes::fFX0A(Chip8 &myChip)
{
    // A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event)
    for (int i = 0; i < 16; i++)
    {
        if (myChip.key[i] != 0)
            myChip.V[(myChip.opCode & 0x0F00) >> 8] = i;
        
    }   
    return;
}

void opCodes::fX15(Chip8 &myChip)
{
    // Sets the delay timer to VX.
    myChip.delayTimer = myChip.V[(myChip.opCode & 0x0F00) >> 8];
    return;
}

void opCodes::fX18(Chip8 &myChip)
{
    // Sets the sound timer to VX.
    myChip.soundTimer = myChip.V[(myChip.opCode & 0x0F00) >> 8];
    return;
}

void opCodes::fX1E(Chip8 &myChip)
{
    // Adds VX to I. VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't
    if ((myChip.I + myChip.V[(myChip.opCode & 0x0F00) >> 8]) > 0xFFF)
        myChip.V[0xF] = 1;
    else
        myChip.V[0xF] = 0;

    myChip.I += myChip.V[(myChip.opCode & 0x0F00) >> 8];
    return;
}

void opCodes::fX29(Chip8 &myChip)
{
    // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
    myChip.I = myChip.V[(myChip.opCode & 0x0F00) >> 8] * 5;
    return;
}

void opCodes::fX33(Chip8 &myChip)
{
    // stores the BCD representation of VX at the address of I, I+1 and I+2
    myChip.memory[myChip.I] = myChip.V[(myChip.opCode & 0x0F00) >> 8] / 100;
    myChip.memory[myChip.I + 1] = (myChip.V[(myChip.opCode & 0x0F00) >> 8] / 10) % 10;
    myChip.memory[myChip.I + 2] = (myChip.V[(myChip.opCode & 0x0F00) >> 8] % 100) % 10;
    return;
}

void opCodes::fFX55(Chip8 &myChip)
{
    // Stores V0 to VX (including VX) in memory starting at address I. 
    // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
    unsigned short tmpX = (myChip.opCode & 0x0F00) >> 8;
    for (short i = 0; i < tmpX; i++)
        myChip.memory[myChip.I + i] = myChip.V[i];
    return;
}

void opCodes::fX65(Chip8 &myChip)
{
    // Fills V0 to VX (including VX) with values from memory starting at address I. 
    // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
    unsigned short tmpX = (myChip.opCode & 0x0F00) >> 8;
    for (short i = 0; i < tmpX; i++)
        myChip.V[i] = myChip.memory[myChip.I + i];
    return;
}


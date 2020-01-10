#pragma once
#include "stdafx.h"
#include <regex>


using namespace std;


    class opCodes {
        public:
            opCodes(Chip8 myChip);
            map<string, int> OPCODES;
            int currOpCodeFunc;

            // OPCODE FUNCTION HANDLES
            void f0NNN(Chip8 &myChip);
            void f00E0(Chip8 &myChip);
            void f00EE(Chip8 &myChip);
            void f1NNN(Chip8 &myChip);
            void f2NNN(Chip8 &myChip);
            void f3XNN(Chip8 &myChip);
            void f4XNN(Chip8 &myChip);
            void f5XY0(Chip8 &myChip);
            void f6XNN(Chip8 &myChip);
            void f7XNN(Chip8 &myChip);
            void f8XY0(Chip8 &myChip);
            void f8XY1(Chip8 &myChip);
            void f8XY2(Chip8 &myChip);
            void f8XY3(Chip8 &myChip);
            void f8XY4(Chip8 &myChip);
            void f8XY5(Chip8 &myChip);
            void f8XY6(Chip8 &myChip);
            void f8XY7(Chip8 &myChip);
            void f8XYE(Chip8 &myChip);
            void f9XY0(Chip8 &myChip);
            void fANNN(Chip8 &myChip);
            void fBNNN(Chip8 &myChip);
            void fCXNN(Chip8 &myChip);
            void fDXYN(Chip8 &myChip);
            void fEX9E(Chip8 &myChip);
            void fEXA1(Chip8 &myChip);
            void fFX07(Chip8 &myChip);
            void fFX0A(Chip8 &myChip);
            void fX15(Chip8 &myChip);
            void fX18(Chip8 &myChip);
            void fX1E(Chip8 &myChip);
            void fX29(Chip8 &myChip);
            void fX33(Chip8 &myChip);
            void fFX55(Chip8 &myChip);
            void fX65(Chip8 &myChip);

            // public class functions
            void executeCurrInstruction(Chip8 );

        
        private:
            // private class functions


    };

typedef void (opCodes::*opCodeMemFn)(Chip8 &myChip);


    opCodes::opCodes(Chip8 myChip)
    {

        OPCODES.insert(pair<string, int>("00e0", 0));
        OPCODES.insert(pair<string, int>("00ee", 1));
        OPCODES.insert(pair<string, int>("1...", 2));
        OPCODES.insert(pair<string, int>("2...", 3));
        OPCODES.insert(pair<string, int>("3...", 4));
        OPCODES.insert(pair<string, int>("4...", 5));
        OPCODES.insert(pair<string, int>("5..0", 6));
        OPCODES.insert(pair<string, int>("6...", 7));
        OPCODES.insert(pair<string, int>("7...", 8));
        OPCODES.insert(pair<string, int>("8..0", 9));
        OPCODES.insert(pair<string, int>("8..1", 10));
        OPCODES.insert(pair<string, int>("8..2", 11));
        OPCODES.insert(pair<string, int>("8..3", 12));
        OPCODES.insert(pair<string, int>("8..4", 13));
        OPCODES.insert(pair<string, int>("8..5", 14));
        OPCODES.insert(pair<string, int>("8..6", 15));
        OPCODES.insert(pair<string, int>("8..7", 16));
        OPCODES.insert(pair<string, int>("8..e", 17));
        OPCODES.insert(pair<string, int>("9..0", 18));
        OPCODES.insert(pair<string, int>("a...", 19));
        OPCODES.insert(pair<string, int>("b...", 20));
        OPCODES.insert(pair<string, int>("c...", 21));
        OPCODES.insert(pair<string, int>("d...", 22));
        OPCODES.insert(pair<string, int>("e.9e", 23));
        OPCODES.insert(pair<string, int>("e.a1", 24));
        OPCODES.insert(pair<string, int>("f.07", 25));
        OPCODES.insert(pair<string, int>("f.0a", 26));
        OPCODES.insert(pair<string, int>("f.15", 27));
        OPCODES.insert(pair<string, int>("f.18", 28));
        OPCODES.insert(pair<string, int>("f.1e", 29));
        OPCODES.insert(pair<string, int>("f.29", 30));
        OPCODES.insert(pair<string, int>("f.33", 31));
        OPCODES.insert(pair<string, int>("f.55", 32));
        OPCODES.insert(pair<string, int>("f.65", 33));
        

        // sort the myChip.opcode into the right function call
        map<string, int>::iterator itr = OPCODES.begin();      

        while (OPCODES.end() != itr)
        {
            regex tmpRegex;
            tmpRegex.assign (itr->first, regex_constants::ECMAScript | regex_constants::icase);

            if (regex_match(myChip.opCodeString, tmpRegex))
            {
                this->currOpCodeFunc = itr->second;
                break;
            }
            itr++;
        }
    }

    
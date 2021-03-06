#pragma once

#include "types.h"

#define MemSize 0x1000
#define StackSize 0x10
#define RegsNum 0x10
#define ProgramStart 0x200

#define ScreenWidth 64
#define ScreenHeight 32
#define FontSize 80

struct chip8 {
    uint8 memory[MemSize];
    uint16 stack[StackSize];
    uint8 V[RegsNum];
    uint16 I;
    uint16 PC;
    uint8 SP;
    uint8 ST;
    uint8 DT;

    uint8 wk;

    uint8 screen[ScreenWidth * ScreenHeight];

    uint8 keys[16];
};

struct chip8 create_chip8();
int load_rom(struct chip8* mac, const char* src);

void chip8_update(struct chip8* mac, char silence);
void update_clock(struct chip8* mac);
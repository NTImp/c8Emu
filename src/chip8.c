#include <stdio.h>
#include "include/chip8.h"
#include "include/hex_font.h"
#include "include/opcodes.h"

void load_hex_sprites(struct chip8* mac);

struct chip8 create_chip8() {
    struct chip8 machine;

    for (int i = 0; i < MemSize; i++)
        machine.memory[i] = 0;
    for (int i = 0; i < StackSize; i++)
        machine.stack[i] = 0;
    for (int i = 0; i < RegsNum; i++)
        machine.V[i] = 0;
    for (int i = 0; i < ScreenWidth * ScreenHeight; i++)
        machine.screen[i] = 0;
    for (int i = 0; i < 16; i++)
        machine.screen[i] = 0;
    machine.I = 0;
    machine.SP = 0;
    machine.PC = ProgramStart;
    machine.ST = 0;
    machine.DT = 0;
    load_hex_sprites(&machine);


    return machine;
}

int load_rom(struct chip8* mac,const char* src) {
    FILE* rom = fopen(src,"rb");
    if (!rom) {
        printf("Cannot open the rom %s\n", src);
        return 0;
    }
    fseek(rom,0,SEEK_END);
    int file_length = ftell(rom);
    fseek(rom,0,SEEK_SET);
    fread(mac->memory + ProgramStart, file_length,1,rom);
    fclose(rom);

    return 1;
}

void load_hex_sprites(struct chip8* mac) {
    for (int i = 0; i < FontSize; i++)
        mac->memory[i] = hex_font[i];
}

void update_clock(struct chip8* mac) {
    if (mac->ST)
        mac->ST--;
    if (mac->DT)
        mac->DT--;
}

void chip8_update(struct chip8* mac, char silence) {
    if(mac->wk) {
        for (int i = 0; i < 16; i++) {
            if(mac->keys[i]) {
                mac->V[mac->wk - 1] = i;
                mac->wk = 0;
            }
        }
    }

    uint16 opcode = (mac->memory[mac->PC] << 8) + (mac->memory[mac->PC + 1]);

    mac->PC = (mac->PC + 2) & 0x0fff;
    execute_opcode(mac,opcode,silence);
}
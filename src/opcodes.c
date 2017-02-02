#include <stdio.h>
#include <stdlib.h>

#include "include/opcodes.h"

typedef void (*opcode_ptr) (struct chip8* machine,uint16);

void op_0xxx(struct chip8* mac, uint16 opcode);
void JP_addr(struct chip8* mac, uint16 opcode); //op_1xxx
void CALL_addr(struct chip8* mac, uint16 opcode); //op_2xxx
void SE_VxByte(struct chip8* mac, uint16 opcode); //op_3xxx
void SNE_VxByte(struct chip8* mac, uint16 opcode); //op_4xxx
void SE_VxVy(struct chip8* mac, uint16 opcode); //op_5xxx
void LD_VxByte(struct chip8* mac, uint16 opcode); //op_6xxx
void ADD_VxByte(struct chip8* mac, uint16 opcode); //op_7xxx
void op_8xxx(struct chip8* mac, uint16 opcode);
void SNE_VxVy(struct chip8* mac, uint16 opcode); //op_9xxx
void LD_Iaddr(struct chip8* mac, uint16 opcode); //op_axxx
void JP_V0addr(struct chip8* mac, uint16 opcode); //op_bxxx
void RND_VxByte(struct chip8* mac, uint16 opcode); //op_cxxx
void DRW_VxVyN(struct chip8* mac, uint16 opcode); //op_dxxx
void op_exxx(struct chip8* mac, uint16 opcode);
void op_fxxx(struct chip8* mac, uint16 opcode);

#define NNN(opcode) (opcode & 0x0fff)
#define N(opcode) (opcode & 0x000f)
#define X(opcode) ((opcode >> 8) & 0x0f)
#define Y(opcode) ((opcode >> 4) & 0x000f)
#define KK(opcode) (opcode & 0x00ff)

void execute_opcode(struct chip8* mac, uint16 opcode) {
    static opcode_ptr opcodes[] = {
        op_0xxx, JP_addr, CALL_addr, SE_VxByte,
        SNE_VxByte, SE_VxVy, LD_VxByte, ADD_VxByte,
        op_8xxx, SNE_VxVy, LD_Iaddr, JP_V0addr,
        RND_VxByte, DRW_VxVyN, op_exxx, op_fxxx
    };

    opcodes[opcode >> 12](mac,opcode);
}

/**
 *opcodes
 */

void CLS(struct chip8* mac, uint16 opcode) {
    //printf("CLS\n");
}
void RET(struct chip8* mac, uint16 opcode) {
    //printf("RET\n");

    mac->PC = mac->stack[mac->SP];
    if (mac->SP)
        mac->SP--;
    else 
        mac->SP = 0xf;
}

void JP_addr(struct chip8* mac, uint16 opcode) {
    //printf("JP\n");
    mac->PC = NNN(opcode);
}

void CALL_addr(struct chip8* mac, uint16 opcode) {
    //printf("CALL\n");
    mac->stack[mac->SP] = mac->PC;
    mac->SP = (mac->SP + 1) % 16;
    mac->PC = NNN(opcode);
}

void SE_VxByte(struct chip8* mac, uint16 opcode) {
    if(mac->V[X(opcode)] == KK(opcode)) 
        mac->PC = (mac->PC + 2) & 0x0fff;
}

void SNE_VxByte(struct chip8* mac, uint16 opcode) {
    if(mac->V[X(opcode)] != KK(opcode))
        mac->PC = (mac->PC + 2) & 0x0fff;
}

void SE_VxVy(struct chip8* mac, uint16 opcode) {
    if(mac->V[X(opcode)] == mac->V[Y(opcode)]) {
        mac->PC += 2;
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void LD_VxByte(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] = KK(opcode);
}

void ADD_VxByte(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] += KK(opcode);
}

void LD_VxVy(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] = mac->V[Y(opcode)];
}

void OR_VxVy(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] |= mac->V[Y(opcode)];
}

void AND_VxVy(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] &= mac->V[Y(opcode)];
}

void XOR_VxVy(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] ^= mac->V[Y(opcode)];
}

void ADD_VxVy(struct chip8* mac, uint16 opcode) {
    if ((int)mac->V[X(opcode)] + mac->V[Y(opcode)] > 0xff)
        mac->V[0xf] = 1;
    else
        mac->V[0xf] = 0;
    mac->V[X(opcode)] = mac->V[X(opcode)] + mac->V[Y(opcode)];

}

void SUB_VxVy(struct chip8* mac, uint16 opcode) {
    if(mac->V[X(opcode)] > mac->V[Y(opcode)])
        mac->V[0xf] = 1;
    else
        mac->V[0xf] = 0;
    mac->V[X(opcode)] = mac->V[X(opcode)] - mac->V[Y(opcode)];
}

void SHR_VxVy(struct chip8* mac, uint16 opcode) {
    mac->V[0xf] = mac->V[X(opcode)] & 0x01;
    
    mac->V[X(opcode)] >>= 1;
}

void SUBN_VxVy(struct chip8* mac, uint16 opcode) {
    if(mac->V[Y(opcode)] > mac->V[X(opcode)])
        mac->V[0xf] = 1;
    else
        mac->V[0xf] = 0;
    mac->V[X(opcode)] = mac->V[Y(opcode)] - mac->V[X(opcode)];
}

void SHL_Vx(struct chip8* mac, uint16 opcode) {
    mac->V[0xf] = mac->V[X(opcode)] >> 7;
    
    mac->V[X(opcode)] <<= 1;
}

void SNE_VxVy(struct chip8* mac, uint16 opcode) {
    if(mac->V[X(opcode)] != mac->V[Y(opcode)]) {
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void LD_Iaddr(struct chip8* mac, uint16 opcode) {
    mac->I = NNN(opcode);
}

void JP_V0addr(struct chip8* mac, uint16 opcode) {
    mac->PC = (NNN(opcode) + mac->V[0]) & 0x0fff;
}

void RND_VxByte(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] = (uint8) rand() & KK(opcode);
}

void DRW_VxVyN(struct chip8* mac, uint16 opcode) {
    mac->V[0xf] = 0;
    uint8 dx = mac->V[X(opcode)];
    uint8 dy = mac->V[Y(opcode)];
    for (int py = 0; py < 5; py++) {
        for (int px = 0; px < 8; px++) {
            uint8 screen_pixel = mac->screen[(py + dy) * 64 + px + dx];
            uint8 sprite_pixel = (mac->memory[mac->I + py] >> (7 - px)) & 0x01;
            uint8 new_screen_pixel = (screen_pixel || sprite_pixel);
            if (screen_pixel && !new_screen_pixel)
                mac->V[0xf] = 0;
            mac->screen[(py + dy) * 64 + px + dx] = new_screen_pixel;
        }
    }
}

void SKP_Vx(struct chip8* mac, uint16 opcode) {
    if(mac->keys & 1 << mac->V[X(opcode)]) {
        mac->PC += 2;
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void SKNP_Vx(struct chip8* mac, uint16 opcode) {
    if(mac->keys & 1 << mac->V[X(opcode)]) {
        mac->PC += 2;
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void LD_VxDT(struct chip8* mac, uint16 opcode) {
    mac->V[X(opcode)] = mac->DT;
}

void LD_VxK(struct chip8* mac, uint16 opcode) {
    mac->wk = 1 + X(opcode);
}

void LD_DtVx(struct chip8* mac, uint16 opcode) {
    mac->DT = mac->V[X(opcode)];
}

void LD_StVx(struct chip8* mac, uint16 opcode) {
    mac->ST = mac->V[X(opcode)];
}

void ADD_IVx(struct chip8* mac, uint16 opcode) {
    mac->I = (mac->V[X(opcode)] + mac->I) & 0x0fff;
}

void LD_FVx(struct chip8* mac, uint16 opcode) {
    mac->I = ((mac->V[X(opcode)] & 0xf) * 5) & 0x0fff;
}

void LD_BVx(struct chip8* mac, uint16 opcode) {
    uint8 number = mac->V[X(opcode)];
    mac->memory[mac->I] = number / 100;
    mac->memory[mac->I + 1] = (number / 10) % 10;
    mac->memory[mac->I + 2] = number % 10;
}

void LD_IVx(struct chip8* mac, uint16 opcode) {
    for (int i = 0; i < X(opcode); i++)
        mac->memory[mac->I + i] = mac->V[i];
}

void LD_VxI(struct chip8* mac, uint16 opcode) {
    for (int i = 0; i < X(opcode); i++)
        mac->V[i] = mac->memory[mac->I + i];
}

void op_0xxx(struct chip8* mac, uint16 opcode) {
    switch(opcode) {
        case 0x00e0:
            CLS(mac,opcode);
            break;
        case 0x00ee:
            RET(mac,opcode);
            break;
    }
}

void op_8xxx(struct chip8* mac, uint16 opcode) {
    switch (N(opcode)) {
        case 0x0:
            LD_VxVy(mac,opcode);
            break;
        case 0x1:
            OR_VxVy(mac,opcode);
            break;
        case 0x2:
            AND_VxVy(mac,opcode);
            break;
        case 0x3:
            XOR_VxVy(mac,opcode);
            break;
        case 0x4:
            ADD_VxVy(mac,opcode);
            break;
        case 0x5:
            SUB_VxVy(mac,opcode);
            break;
        case 0x6:
            SHR_VxVy(mac,opcode);
            break;
        case 0x7:
            SUBN_VxVy(mac,opcode);
            break;
        case 0xe:
            SHL_Vx(mac,opcode);
            break;
    }
}

void op_exxx(struct chip8* mac, uint16 opcode) {
    switch(KK(opcode)) {
        case 0x9e:
            SKP_Vx(mac,opcode);
            break;
        case 0xa1:
            SKNP_Vx(mac,opcode);
            break;
    }
}

void op_fxxx(struct chip8* mac, uint16 opcode) {
    switch(KK(opcode)) {
        case 0x07:
            LD_VxDT(mac,opcode);
            break;
        case 0x0a:
            LD_VxK(mac,opcode);
            break;
        case 0x15:
            LD_DtVx(mac,opcode);
            break;
        case 0x18:
            LD_StVx(mac,opcode);
            break;
        case 0x1e:
            ADD_IVx(mac,opcode);
            break;
        case 0x29:
            LD_FVx(mac,opcode);
            break;
        case 0x33:
            LD_BVx(mac,opcode);
            break;
        case 0x55:
            LD_IVx(mac,opcode);
            break;
        case 0x65:
            LD_VxI(mac,opcode);
            break;
    }
}
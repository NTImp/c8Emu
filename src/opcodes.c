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

#define NNN(opcode) (opcode & 0xfff)
#define N(opcode) (opcode & 0x0f)
#define X(opcode) ((opcode >> 8) & 0x0f)
#define Y(opcode) ((opcode >> 4) & 0x0f)
#define KK(opcode) (opcode & 0xff)

char silenced = 1;

void log(const char* txt) {
    if (!silenced) {
        printf(txt);
        printf("\n");
    }
}

void execute_opcode(struct chip8* mac, uint16 opcode, char s) {
    static opcode_ptr opcodes[] = {
        op_0xxx, //0
        JP_addr, //1
        CALL_addr, //2
        SE_VxByte, //3
        SNE_VxByte, //4
        SE_VxVy, //5
        LD_VxByte, //6
        ADD_VxByte, //7
        op_8xxx, //8
        SNE_VxVy, //9
        LD_Iaddr, //a
        JP_V0addr, //b
        RND_VxByte, //c
        DRW_VxVyN, //d
        op_exxx, //e
        op_fxxx //f
    };
    silenced = s;
    opcodes[opcode >> 12](mac,opcode);
}

/**
 *opcodes
 */

void CLS(struct chip8* mac, uint16 opcode) {
    log("CLS");
    for (int i=0; i < 32*64; i++) {
        mac->screen[i] = 0;
    }
}
void RET(struct chip8* mac, uint16 opcode) {
    log("RET");

    mac->PC = mac->stack[mac->SP];
    mac->SP--;
}

void JP_addr(struct chip8* mac, uint16 opcode) {
    log("JP");
    mac->PC = NNN(opcode);
}

void CALL_addr(struct chip8* mac, uint16 opcode) {
    log("CALL");
    mac->SP++;
    mac->stack[mac->SP] = mac->PC;
    mac->PC = NNN(opcode);
}

void SE_VxByte(struct chip8* mac, uint16 opcode) {
    log("SE Vx byte");
    if(mac->V[X(opcode)] == KK(opcode)) 
        mac->PC = (mac->PC + 2) & 0x0fff;
}

void SNE_VxByte(struct chip8* mac, uint16 opcode) {
    log("SNE Vx byte");
    if(mac->V[X(opcode)] != KK(opcode))
        mac->PC = (mac->PC + 2) & 0x0fff;
}

void SE_VxVy(struct chip8* mac, uint16 opcode) {
    log("SE Vx Vy");
    if(mac->V[X(opcode)] == mac->V[Y(opcode)]) {
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void LD_VxByte(struct chip8* mac, uint16 opcode) {
    log("LD Vx byte");
    mac->V[X(opcode)] = KK(opcode);
}

void ADD_VxByte(struct chip8* mac, uint16 opcode) {
    log("ADD Vx byte");
    mac->V[X(opcode)] += KK(opcode);
}

void LD_VxVy(struct chip8* mac, uint16 opcode) {
    log("LD Vx Vy");
    mac->V[X(opcode)] = mac->V[Y(opcode)];
}

void OR_VxVy(struct chip8* mac, uint16 opcode) {
    log("OR Vx Vy");
    mac->V[X(opcode)] |= mac->V[Y(opcode)];
}

void AND_VxVy(struct chip8* mac, uint16 opcode) {
    log("AND Vx Vy");
    mac->V[X(opcode)] &= mac->V[Y(opcode)];
}

void XOR_VxVy(struct chip8* mac, uint16 opcode) {
    log("XOR Vx Vy");
    mac->V[X(opcode)] ^= mac->V[Y(opcode)];
}

void ADD_VxVy(struct chip8* mac, uint16 opcode) {
    log("ADD Vx Vy");
    mac->V[0xf] = mac->V[X(opcode)] > ((mac->V[X(opcode)] + mac->V[Y(opcode)]) & 0xff);
    mac->V[X(opcode)] += mac->V[Y(opcode)];

}

void SUB_VxVy(struct chip8* mac, uint16 opcode) {
    log("SUB Vx Vy");
    mac->V[0xf] = mac->V[X(opcode)] > mac->V[Y(opcode)];
    mac->V[X(opcode)] -= mac->V[Y(opcode)];
}

void SHR_VxVy(struct chip8* mac, uint16 opcode) {
    log("SHR Vx [Vy]");
    mac->V[0xf] = mac->V[X(opcode)] & 0x01;
    
    mac->V[X(opcode)] >>= 1;
}

void SUBN_VxVy(struct chip8* mac, uint16 opcode) {
    log("SUBN Vx Vy");
    mac->V[0xf] = mac->V[Y(opcode)] > mac->V[X(opcode)];
    mac->V[X(opcode)] = mac->V[Y(opcode)] - mac->V[X(opcode)];
}

void SHL_Vx(struct chip8* mac, uint16 opcode) {
    log("SHL Vx");
    mac->V[0xf] = mac->V[X(opcode)] & 0x80;
    mac->V[X(opcode)] <<= 1;
}

void SNE_VxVy(struct chip8* mac, uint16 opcode) {
    log("SNE Vx Vy");
    if(mac->V[X(opcode)] != mac->V[Y(opcode)]) {
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void LD_Iaddr(struct chip8* mac, uint16 opcode) {
    log("LD I addr");
    mac->I = NNN(opcode);
}

void JP_V0addr(struct chip8* mac, uint16 opcode) {
    log("JP V0 addr");
    mac->PC = (NNN(opcode) + mac->V[0]) & 0x0fff;
}

void RND_VxByte(struct chip8* mac, uint16 opcode) {
    log("RND Vx byte");
    mac->V[X(opcode)] = rand() & KK(opcode);
}

void DRW_VxVyN(struct chip8* mac, uint16 opcode) {
    log("DRW Vx Vy N");
    mac->V[0xf] = 0;
    uint8 dx = mac->V[X(opcode)];
    uint8 dy = mac->V[Y(opcode)];
    for (int py = 0; py < N(opcode); py++) {
        for (int px = 0; px < 8; px++) {
            uint8 screen_xpos = (px + dx) % 64;
            uint8 screen_ypos = (py + dy) % 32;
            uint8 screen_pixel = mac->screen[screen_ypos * 64 + screen_xpos];
            uint8 sprite_pixel = (mac->memory[mac->I + py] >> (7 - px)) & 0x01;
            uint8 new_screen_pixel = (screen_pixel != sprite_pixel);
            if (screen_pixel && !new_screen_pixel)
                mac->V[0xf] = 1;
            mac->screen[screen_ypos * 64 + screen_xpos] = new_screen_pixel;
        }
    }
}

void SKP_Vx(struct chip8* mac, uint16 opcode) {
    log("SKP Vx");
    if(mac->keys[mac->V[X(opcode)]]) {
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void SKNP_Vx(struct chip8* mac, uint16 opcode) {
    log("SKNP Vx");
    if(!mac->keys[mac->V[X(opcode)] & 0xf]) {
        mac->PC = (mac->PC + 2) & 0x0fff;
    }
}

void LD_VxDT(struct chip8* mac, uint16 opcode) {
    log("LD Vx DT");
    mac->V[X(opcode)] = mac->DT;
}

void LD_VxK(struct chip8* mac, uint16 opcode) {
    log("LD Vx K");
    mac->wk = 1 + X(opcode);
}

void LD_DtVx(struct chip8* mac, uint16 opcode) {
    log("LD DT Vx");
    mac->DT = mac->V[X(opcode)];
}

void LD_StVx(struct chip8* mac, uint16 opcode) {
    log("LD ST Vx");
    mac->ST = mac->V[X(opcode)];
}

void ADD_IVx(struct chip8* mac, uint16 opcode) {
    log("ADD I Vx");
    mac->I = (mac->V[X(opcode)] + mac->I) & 0x0fff;
}

void LD_FVx(struct chip8* mac, uint16 opcode) {
    log("LD F Vx");
    mac->I = (mac->V[X(opcode)] & 0xf) * 5;
}

void LD_BVx(struct chip8* mac, uint16 opcode) {
    log("LD B Vx");
    uint8 number = mac->V[X(opcode)];
    mac->memory[mac->I] = number / 100;
    mac->memory[mac->I + 1] = (number / 10) % 10;
    mac->memory[mac->I + 2] = number % 10;
}

void LD_IVx(struct chip8* mac, uint16 opcode) {
    log("LD I Vx");
    for (int i = 0; i <= X(opcode); i++)
        mac->memory[mac->I + i] = mac->V[i];
}

void LD_VxI(struct chip8* mac, uint16 opcode) {
    log("LD Vx I");
    for (int i = 0; i <= X(opcode); i++)
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
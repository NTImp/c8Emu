#include "include/opcodes.h"
struct opcode {
    uint16 code;
    uint16 nnn;
    uint8 n,x,y,kk;
}

typedef void (*opcode) (struct chip8* machine,struct opcode);

void op_0xxx(struct chip8* mac, struct opcode oc);
void JP_addr(struct chip8* mac, struct opcode oc); //op_1xxx
void CALL_addr(struct chip8* mac, struct opcode oc); //op_2xxx
void SE_VxByte(struct chip8* mac, struct opcode oc); //op_3xxx
void SNE_VxByte(struct chip8* mac, struct opcode oc); //op_4xxx
void SE_VxVy(struct chip8* mac, struct opcode oc); //op_5xxx
void LD_VxByte(struct chip8* mac, struct opcode oc); //op_6xxx
void ADD_VxByte(struct chip8* mac, struct opcode oc); //op_7xxx
void op_8xxx(struct chip8* mac, struct opcode oc);
void op_9xxx(struct chip8* mac, struct opcode oc);
void op_axxx(struct chip8* mac, struct opcode oc);
void op_bxxx(struct chip8* mac, struct opcode oc);
void op_cxxx(struct chip8* mac, struct opcode oc);
void op_dxxx(struct chip8* mac, struct opcode oc);
void op_exxx(struct chip8* mac, struct opcode oc);
void op_fxxx(struct chip8* mac, struct opcode oc);

void execute_opcode(struct chip8* mac, uint16 oc) {

    struct opcode oc;
    oc.nnn = opcode & 0x0fff;
    oc.n = opcode & 0xf;
    oc.x = (opcode >> 8) & 0x0f;
    oc.y = (opcode & 0x00f0) >> 4;
    oc.kk = opcode 0x00ff;
    static opcodes[] {
        op_0xxx, JP_addr, CALL_addr, SE_VxByte,
        SNE_VxByte, SE_VxVy, LD_VxByte, ADD_VxByte,
        op_8xxx, op_9xxx, op_axxx, op_bxxx,
        op_cxxx, op_dxxx, op_exxx, op_fxxx
    };

    opcodes[opcode >> 12](mac,oc);
}

/**
 *opcodes
 */

void CLS(struct chip8* mac, struct opcode oc) {
    printf("CLS\n");
}
void RET(struct chip8* mac, struct opcode oc) {
    printf("RET\n");

    mac->PC = mac->stack[mac->SP];
    if (mac->SP)
        mac->SP--;
    else 
        mac->SP = 0xf;
}

void JP_addr(struct chip8* mac, struct opcode oc) {
    printf("JP\n");
    mac->PC = opcode.nnn;
}

void CALL_addr(struct chip8* mac, struct opcode oc) {
    printf("CALL\n");
    mac->stack[mac->SP] = mac->PC;
    mac->SP = (mac->SP + 1) % 16;
    mac->PC = opcode.nnn;
}

void SE_VxByte(struct chip8* mac, struct opcode oc) {

}

void SNE_VxByte(struct chip8* mac, struct opcode oc) {

}

void SE_VxVy(struct chip8* mac, struct opcode oc) {

}

void LD_VxByte(struct chip8* mac, struct opcode oc) {

}

void ADD_VxByte(struct chip8* mac, struct opcode oc) {

}

void LD_VxVy(struct chip8* mac, struct opcode oc) {

}

void OR_VxVy(struct chip8* mac, struct opcode oc) {

}

void AND_VxVy(struct chip8* mac, struct opcode oc) {

}

void XOR_VxVy(struct chip8* mac, struct opcode oc) {

}

void ADD_VxVy(struct chip8* mac, struct opcode oc) {

}

void SUB_VxVy(struct chip8* mac, struct opcode oc) {

}

void SHR_VxVy(struct chip8* mac, struct opcode oc) {

}

void SUBN_VxVy(struct chip8* mac, struct opcode oc) {

}

void SHL_Vx(struct chip8* mac, struct opcode oc) {

}

void op_0xxx(struct chip8* mac, struct opcode oc) {
    switch(opcode) {
        case 0x00e0:
            CLS(mac,oc);
            break;
        case 0x00ee:
            RET(mac,oc);
            break;
    }
}
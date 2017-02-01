#include<stdio.h>

#include "include/chip8.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("You should pass a rom\n");
        return 0;
    }

    struct chip8 machine = create_chip8();
    load_rom(&machine, argv[1]);

    while(1) {
        chip8_update(&machine);
    }

    return 0;
}
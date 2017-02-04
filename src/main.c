#include<stdio.h>

#include "include/sdl.h"
#include "include/chip8.h"
#include "include/opcodes.h"
#include "include/sdl.h"

#define EmulatorRunning 1
#define EmulatorPaused 0
#define EmulatorStop -1

void PollEvents();
void InitEmulator();

char quit = 0;
char emulator_status = EmulatorStop;
const char* rom = 0;

struct chip8 machine;

int main(int argc, char** argv) {
    if (argc >= 1) {
        rom = argv[1];
    }

    if(!InitSDL())
        return -1;

    InitEmulator();

    Uint32 clock_ticks = SDL_GetTicks();
    Uint32 op_ticks = SDL_GetTicks();
    while(!quit) {
        PollEvents();        

        if (emulator_status == EmulatorRunning) {
            if(SDL_GetTicks() - clock_ticks > 1000/60) {
                update_clock(&machine);
                clock_ticks = SDL_GetTicks();
            }

            if (SDL_GetTicks() - op_ticks > 2) {
                C8Updatekeys(machine.keys);
                chip8_update(&machine, 1);

                FillScreen(machine.screen);
                DrawScreen();

                op_ticks = SDL_GetTicks();
            }
        }
    }

    ExitSDL();

    return 0;
}

void InitEmulator() {
    if (rom) {
        machine = create_chip8();
        if(load_rom(&machine, rom)) {
            emulator_status = EmulatorRunning;
            return;
        }
    }
    emulator_status = EmulatorStop;
}

void PollEvents() {
    SDL_Event ev;
    while(SDL_PollEvent(&ev)) {
        char key;
        switch (ev.type) {
        case SDL_QUIT:
            quit = 1;
            break;
        case SDL_KEYDOWN:
            key = ev.key.keysym.scancode;
            if(!ev.key.repeat) {
                switch (key) {
                case SDL_SCANCODE_RETURN:
                    if (emulator_status == EmulatorRunning) {
                        emulator_status = EmulatorPaused;
                    } else if (emulator_status == EmulatorPaused) {
                        emulator_status = EmulatorRunning;
                    }
                    break;
                case SDL_SCANCODE_SPACE:
                    InitEmulator();
                }
            }
            break;
        }
    }
}
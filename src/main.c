#include<stdio.h>

#include "include/sdl.h"
#include "include/chip8.h"
#include "include/opcodes.h"
#include "include/sdl.h"

#define MAX_SPEED 10

enum EmulatorStatus {
	emulator_running, emulator_paused, emulator_stop
};

void PollEvents();
void InitEmulator();

char speed = MAX_SPEED - 1;
char emulator_status = -1;
const char* rom = 0;

struct chip8 machine;

int main(int argc, char** argv)
{
	if (argc >= 1) {
		rom = argv[1];
	}

	if(!InitSDL())
	return -1;

	InitEmulator();

	Uint32 clock_ticks = SDL_GetTicks();
	Uint32 op_ticks = SDL_GetTicks();
	while(emulator_status != emulator_stop) {
		PollEvents();        

		if (emulator_status == emulator_running) {
			if(SDL_GetTicks() - clock_ticks > 1000/60) {
				update_clock(&machine);
				clock_ticks = SDL_GetTicks();
			}

			if (SDL_GetTicks() - op_ticks > MAX_SPEED - speed) {
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

void InitEmulator()
{
    if (rom) {
        machine = create_chip8();
        if (load_rom(&machine, rom)) {
            emulator_status = emulator_running;
            return;
        }
    } else {
	printf("You should pass a rom as parameter\n");
    }
    emulator_status = emulator_stop;
}

void PollEvents()
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        char key;
        switch (ev.type) {
        case SDL_QUIT:
		emulator_status = emulator_stop;
		break;
        case SDL_KEYDOWN:
            key = ev.key.keysym.scancode;

            if(!ev.key.repeat) {
                switch (key) {
                case SDL_SCANCODE_RETURN:

			if (emulator_status == emulator_running) {
				emulator_status = emulator_paused;
			}
			else if (emulator_status == emulator_paused) {
				emulator_status = emulator_running;
			}

			break;
                case SDL_SCANCODE_ESCAPE:
			InitEmulator();
			break;
		case SDL_SCANCODE_UP:
			speed = (speed + 1) % MAX_SPEED;	
			printf("Speed is: %d/9\n");
			break;
		case SDL_SCANCODE_DOWN:
			speed = (speed + MAX_SPEED - 1) % MAX_SPEED;	
			printf("Speed is: %d/9\n");
			break;
		}
            }
            break;
        }
    }
}

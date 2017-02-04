#include<stdio.h>

#include "include/sdl.h"
#include "include/chip8.h"
#include "include/opcodes.h"
#include "include/sdl.h"



int main(int argc, char** argv) {
    if (argc < 2) {
        printf("You should pass a rom\n");
        return 0;
    }

    struct chip8 machine = create_chip8();
    load_rom(&machine, argv[1]);

    InitSDL();

    SDL_Window* win = GetWindow();
    SDL_Renderer* render = GetRender();
    if(!win || !render)
        return -1;

    char exit = 0;
    SDL_Event ev;
    char pause = 0;

    while(!exit) {
        while(SDL_PollEvent(&ev)) {
            char key;
            switch (ev.type) {
            case SDL_QUIT:
                exit = 1;
                break;
            case SDL_KEYDOWN:
                key = ev.key.keysym.scancode;
                if(!ev.key.repeat) {
                    switch (key) {
                    case SDL_SCANCODE_RETURN:
                        pause = !pause;
                        break;
                    }
                }
                break;
            }
        }

        if (!pause) {
            C8Updatekeys(machine.keys);

            chip8_update(&machine, 1);

            FillScreen(machine.screen);
            DrawScreen();

            SDL_Delay(5);
        }
        
    }

    ExitSDL();

    return 0;
}
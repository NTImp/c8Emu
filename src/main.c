#include<stdio.h>
#include<SDL2/SDL.h>

#include "include/chip8.h"
#include "include/opcodes.h"

SDL_Window* win = 0;
SDL_Renderer* render = 0;
SDL_Texture* c8_screen;

uint8 c8_keymap[16] = {
    SDL_SCANCODE_X, //0
    SDL_SCANCODE_1, //1
    SDL_SCANCODE_2, //2
    SDL_SCANCODE_3, //3
    SDL_SCANCODE_Q, //4
    SDL_SCANCODE_W, //5
    SDL_SCANCODE_E, //6
    SDL_SCANCODE_A, //7
    SDL_SCANCODE_S, //8
    SDL_SCANCODE_D, //9
    SDL_SCANCODE_Z, //a
    SDL_SCANCODE_C, //b
    SDL_SCANCODE_4, //c
    SDL_SCANCODE_R, //d
    SDL_SCANCODE_F, //e
    SDL_SCANCODE_V  //f
};

void C8Updatekeys(struct chip8* mac) {
    const Uint8* skeys = SDL_GetKeyboardState(NULL);
    for (int i = 0; i < 16; i++) {
        mac->keys[i] = skeys[c8_keymap[i]];
    }
}

void InitSDL() {
    SDL_Init(SDL_INIT_EVERYTHING);

    win = SDL_CreateWindow("c8EMU",0,0,640,480,SDL_WINDOW_SHOWN);
    if(!win) {
        printf("Cannot create window\n");
        SDL_Quit();
        return;
    }
    render = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
    if(!render) {
        printf("Cannot create renderer\n");
        SDL_DestroyWindow(win);
        win = 0;
        SDL_Quit();
        return;
    }

    c8_screen = SDL_CreateTexture(render,SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

void ExitSDL() {
    SDL_DestroyTexture(c8_screen);

    SDL_DestroyRenderer(render);
    render = 0;
    SDL_DestroyWindow(win);
    win = 0;
    SDL_Quit();
    return;
}

void FillScreen(uint8 *screen) {
    Uint32* pixels = 0;
    int pitch;
    SDL_LockTexture(c8_screen, NULL,(void**)&pixels,&pitch);
    for (int i=0;i< 64*32; i++) {
        if(screen[i]) {
            pixels[i] = -1;
        } else  {
            pixels[i] = 0;
        }
    }
    SDL_UnlockTexture(c8_screen);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("You should pass a rom\n");
        return 0;
    }

    struct chip8 machine = create_chip8();
    load_rom(&machine, argv[1]);

    InitSDL();

    if(!win || !render)
        return -1;
    char exit = 0;
    SDL_Event ev;

    while(!exit) {
        while(SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                exit = 1;
                break;
            }
        }
        C8Updatekeys(&machine);

        chip8_update(&machine, 1);

        FillScreen(machine.screen);
        SDL_RenderCopy(render, c8_screen, 0, 0);
        SDL_RenderPresent(render);

        SDL_Delay(2);
    }

    ExitSDL();

    return 0;
}
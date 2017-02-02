#include<stdio.h>
#include<SDL2/SDL.h>

#include "include/chip8.h"
#include "include/opcodes.h"

SDL_Window* win = 0;
SDL_Renderer* render = 0;
SDL_Texture* c8_screen;

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
        machine.keys = 0;
        chip8_update(&machine,0);

        FillScreen(machine.screen);
        SDL_RenderCopy(render, c8_screen, 0, 0);
        SDL_RenderPresent(render);
    }

    ExitSDL();

    return 0;
}
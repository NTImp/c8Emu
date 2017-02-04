#pragma once
#include "types.h"
#include <SDL2/SDL.h>

void C8Updatekeys(uint8* keys);
int InitSDL();
void ExitSDL();
void FillScreen(uint8 *screen);

void DrawScreen();

SDL_Window* GetWindow();
SDL_Renderer* GetRender();

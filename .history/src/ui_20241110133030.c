#include <SDL2/SDL.h>
#include <stdio.h>

int initialise_ui() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return 1;
    }
}
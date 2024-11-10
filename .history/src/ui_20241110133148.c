#include <SDL2/SDL.h>
#include <stdio.h>

int initialise_ui() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("EduLogic: Digital Circuit Simulator",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("SDL Window Error: %s\n", SDL_GetError());
    }
}
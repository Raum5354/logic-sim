#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int initialize_ui(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create the SDL window
    SDL_Window *window = SDL_CreateWindow("EduLogic: Circuit Simulator",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        printf("SDL Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create the SDL renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop to keep the window open and draw gates
    SDL_Event event;
    int running = 1;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen with a white background 
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SLD_RenderClear(renderer);

        // Set colour to black for drawing shapes
        SDL_SetRenderDrawColor(renderer, 0, 0 , 0, 255);

        // Draw a rectangle for the AND gate
        SDL_Rect andGate = {100, 100, 100, 50}; // x, y, width, height
        SDL_RenderFillRect(renderer, &andGate);

        // Draw a rectangle for the OR gate
        SDL_Rect orGate = {250, 100, 100, 50};
        SDL_RenderFillRect(renderer, &orGate);

        // Draw a circle for the NOTE gate
        int centerX = 450;
        int centerY = 125; 
        int radius = 25;
        for (int width = 0; width < radius * 2; width++) {
            for (int height = 0; height < radius * 2)
        }


    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

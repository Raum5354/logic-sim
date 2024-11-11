#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void draw_and_gate(SDL_Renderer *renderer, int x, int y);
void draw_or_gate(SDL_Renderer *renderer, int x, int y);
void draw_not_gate(SDL_Renderer *renderer, int x, int y);

int initialize_ui(void) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create the SDL window
    SDL_Window *window = SDL_CreateWindow("Logic Sim: Digital Circuit Simulator",
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
        SDL_RenderClear(renderer);

        // Set colour to black for drawing shapes
        SDL_SetRenderDrawColor(renderer, 0, 0 , 0, 255);

        // Draw each gate at a specified position
        draw_and_gate(renderer, 200, 300);
        draw_or_gate(renderer, 300, 300);
        draw_not_gate(renderer, 500, 300);

        // Present the rendered content
        SDL_RenderPresent(renderer);
    }

    // Cleanup and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void draw_and_gate(SDL_Renderer *renderer, int x, int y) {
    // Draw the rectangular left part
    SDL_RenderDrawLine(renderer, x, y, x, y + 50); // Left vertical line
    SDL_RenderDrawLine(renderer, x, y, x + 25, y); // Top horizontal line
    SDL_RenderDrawLine(renderer, x, y + 50, x + 25, y + 50); // Bottom horizontal line

    // Draw the semi-circle on the right side
    int centerX = x + 25;
    int centerY = y + 25;
    int radius = 25;
    for (int angle = -90; angle <= 90; angle++) {
        int dx = (int)(radius * cos(angle * M_PI / 180.0));
        int dy = (int)(radius * sin(angle * M_PI / 180.0));
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
    }

    // Draw input lines
    SDL_RenderDrawLine(renderer, x - 15, y + 10, x, y + 10); // Top input
    SDL_RenderDrawLine(renderer, x - 15, y + 40, x, y + 40); // Bottom input

    // Draw output line
    SDL_RenderDrawLine(renderer, centerX + radius, centerY, centerX + radius + 15, centerY);
}

void draw_or_gate(SDL_Renderer *renderer, int x, int y) {
    int width = 15;   // Width of the OR gate
    int height = 40;  // Height of the OR gate
    int radius = height / 2; // Radius of the circles

    // Coordinates of the rectangle (centered at (x, y))
    int rect_x = x + width / 2;
    int rect_y = y + height / 2;

    // // Draw the rectangle
    // SDL_Rect rect = { rect_x, rect_y, width, height };
    // SDL_RenderDrawRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, x-radius*0.5, y-height/2, x+radius, y-height/2); // Top horizontal line
    SDL_RenderDrawLine(renderer, x-radius*0.5, y+height/2, x+radius, y+height/2); // Bottom horizontal line

    // Number of points for smooth circles
    int num_points = 100;

    // Draw the right semicircle (only the right half protruding)
    int center_right_x = x + width;
    int center_y = y;
    SDL_Point right_circle_points[num_points];
    for (int i = 0; i < num_points; i++) {
        double theta = M_PI / 2 - (M_PI * i) / (num_points - 1); // From 90 degrees to -90 degrees
        int x_pos = center_right_x + radius * cos(theta);
        int y_pos = center_y - radius * sin(theta);
        right_circle_points[i] = (SDL_Point){ x_pos, y_pos };
    }
    SDL_RenderDrawLines(renderer, right_circle_points, num_points);

    // Draw the left semicircle (only the left half inside the rectangle)
    int center_left_x = x - width;
    SDL_Point left_circle_points[num_points];
    for (int i = 0; i < num_points; i++) {
        double theta = M_PI / 2 - (M_PI * i) / (num_points - 1); // From 90 degrees to -90 degrees
        int x_pos = center_left_x + radius * cos(theta);
        int y_pos = center_y - radius * sin(theta);
        left_circle_points[i] = (SDL_Point){ x_pos, y_pos };
    }
    SDL_RenderDrawLines(renderer, left_circle_points, num_points);

    // Draw input lines extending to the edge of the left semicircle
    int input_y_offset = height / 4;

    // Calculate exact tangent points on the left semicircle for the inputs
    int tangent_x_top = center_left_x + (int)(radius * cos(M_PI / 4)); // 45 degrees
    int tangent_y_top = y - input_y_offset;

    int tangent_x_bottom = center_left_x + (int)(radius * cos(M_PI / 4)); // 45 degrees
    int tangent_y_bottom = y + input_y_offset;

    SDL_RenderDrawLine(renderer, tangent_x_top - 15, tangent_y_top, tangent_x_top, tangent_y_top); // Top input line
    SDL_RenderDrawLine(renderer, tangent_x_bottom - 15, tangent_y_bottom, tangent_x_bottom, tangent_y_bottom); // Bottom input line

    // Draw output line extending from the right semicircle
    SDL_RenderDrawLine(renderer, center_right_x + radius, y, center_right_x + radius + 15, y);
}


void draw_not_gate(SDL_Renderer *renderer, int x, int y) {
    // Draw the triangle for the NOT gate outline
    SDL_RenderDrawLine(renderer, x, y + 10, x + 30, y + 25);      // Left side
    SDL_RenderDrawLine(renderer, x, y + 40, x + 30, y + 25);      // Right side
    SDL_RenderDrawLine(renderer, x, y + 10, x, y + 40);           // Vertical side

    // Draw the outline of the inversion bubble at the tip of the triangle
    int bubbleCenterX = x + 35; // Positioning at the triangle tip
    int bubbleCenterY = y + 25;
    int bubbleRadius = 5;

    for (int angle = 0; angle < 360; angle++) {
        int dx = (int)(bubbleRadius * cos(angle * M_PI / 180.0));
        int dy = (int)(bubbleRadius * sin(angle * M_PI / 180.0));
        SDL_RenderDrawPoint(renderer, bubbleCenterX + dx, bubbleCenterY + dy);
    }

    // Draw input line
    SDL_RenderDrawLine(renderer, x - 15, y + 25, x, y + 25); // Input line to the left

    // Draw output line
    SDL_RenderDrawLine(renderer, bubbleCenterX + bubbleRadius, bubbleCenterY, bubbleCenterX + bubbleRadius + 15, bubbleCenterY);
}

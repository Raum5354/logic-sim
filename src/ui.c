#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void draw_and_gate(SDL_Renderer *renderer, int x, int y);
void draw_or_gate(SDL_Renderer *renderer, int x, int y);
void draw_not_gate(SDL_Renderer *renderer, int x, int y);
double get_x_on_circle(int x_center, int y_center, int radius, int y);

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
    int width = 20;   // Width of the OR gate
    int height = 40;  // Height of the OR gate

    // Draw the rectangular left part
    SDL_RenderDrawLine(renderer, x, y - height / 2, x, y + height / 2); // Left vertical line
    SDL_RenderDrawLine(renderer, x, y - height / 2, x + width, y - height / 2); // Top horizontal line
    SDL_RenderDrawLine(renderer, x, y + height / 2, x + width, y + height / 2); // Bottom horizontal line

    // Draw the semi-circle on the right side
    int radius = height / 2;
    int center_x = x + width;
    int center_y = y;
    
    for (int angle = -90; angle <= 90; angle++) {
        int dx = (int)(radius * cos(angle * M_PI / 180.0));
        int dy = (int)(radius * sin(angle * M_PI / 180.0));
        SDL_RenderDrawPoint(renderer, center_x + dx, center_y + dy);
    }

    // Draw input lines
    SDL_RenderDrawLine(renderer, x - width, y - height / 4, x, y - height / 4); // Top input
    SDL_RenderDrawLine(renderer, x - width, y + height / 4, x, y + height / 4); // Bottom input

    // Draw output line
    SDL_RenderDrawLine(renderer, center_x + radius, center_y, center_x + radius + 15, center_y);
}

void draw_or_gate(SDL_Renderer *renderer, int x, int y) {
    int width = 10;   // Width of the OR gate
    int height = 40;  // Height of the OR gate
    int radius_right = height / 2; // Radius of the right circle

    // Number of points for smooth circles
    int num_points = 100;

    // Draw the right semicircle (only the right half protruding)
    int center_x_right = x + width;
    SDL_Point right_circle_points[num_points];
    for (int i = 0; i < num_points; i++) {
        double theta = M_PI / 2 - (M_PI * i) / (num_points - 1); // From 90 degrees to -90 degrees
        int x_pos = center_x_right + radius_right * cos(theta);
        int y_pos = y - radius_right * sin(theta);
        right_circle_points[i] = (SDL_Point){ x_pos, y_pos };
    }
    SDL_RenderDrawLines(renderer, right_circle_points, num_points);

    // Draw the left semicircle 
    int radius_left = height / 2.5; // Radius of the left circle
    int center_x_left = x - width;
    SDL_Point left_circle_points[num_points];
    for (int i = 0; i < num_points; i++) {
        double theta = M_PI / 2 - (M_PI * i) / (num_points - 1); // From 90 degrees to -90 degrees
        int x_pos = center_x_left + radius_left * cos(theta);
        int y_pos = y - radius_left * sin(theta);
        left_circle_points[i] = (SDL_Point){ x_pos, y_pos };
    }
    SDL_RenderDrawLines(renderer, left_circle_points, num_points);


    /// graphics library: https://discourse.libsdl.org/t/sdl2-gfx-extensions/26574

    // Connect the semi-circles
    int rect_x = x + width / 2;
    int rect_y = y + height / 2;

    double y_max_circle_left = y + radius_left;
    double y_min_circle_left = y - radius_left;

    double y_max_circle_right = y + radius_right;
    double y_min_circle_right = y - radius_right;

    double x_1 = get_x_on_circle(center_x_left, y, radius_left, y_max_circle_left);
    double x_2 = get_x_on_circle(center_x_right, y, radius_right, y_max_circle_right);

    SDL_RenderDrawLine(renderer, x_1, y_max_circle_left, x_2, y_max_circle_right); // Top line
    SDL_RenderDrawLine(renderer, x_1, y_min_circle_left, x_2, y_min_circle_right); // Bottom line

    // Drawing the input lines
    double y_top_input_line = y + radius_left / 2;
    double y_bottom_input_line = y - radius_left / 2;
    // Could have been y_bottom_input_line instead of y_top_input_line ∵ both give the same x value
    double x_input_lines = get_x_on_circle(center_x_left, y, radius_left, y_top_input_line);

    SDL_RenderDrawLine(renderer, x_input_lines - 15, y_top_input_line, x_input_lines, y_top_input_line); // Top input line
    SDL_RenderDrawLine(renderer, x_input_lines - 15, y_bottom_input_line, x_input_lines, y_bottom_input_line); // Bottom input line

    // Draw output line extending from the right semicircle
    SDL_RenderDrawLine(renderer, center_x_right + radius_right, y, center_x_right + radius_left + 15, y);
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

double get_x_on_circle(int x_center, int y_center, int radius, int y) {
    // Only take the solutions for the right semi-circle
    return x_center + sqrt(radius * radius - (y - y_center) * (y - y_center));
}
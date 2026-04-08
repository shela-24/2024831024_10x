#include <SDL3/SDL.h>
#include <math.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600

void drawCircle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    for (int w = -radius; w <= radius; w++) {
        for (int h = -radius; h <= radius; h++) {
            if (w*w + h*h <= radius*radius)
                SDL_RenderPoint(renderer, cx + w, cy + h);
        }
    }
}

int main() {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Task 102", WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    bool running = true;
    SDL_Event event;

    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;
    int radius = 20;
    int initialRadius = 20;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        drawCircle(renderer, centerX, centerY, radius);

        radius++;

        if (centerX + radius >= WIDTH || centerX - radius <= 0 ||
            centerY + radius >= HEIGHT || centerY - radius <= 0) {
            radius = initialRadius;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

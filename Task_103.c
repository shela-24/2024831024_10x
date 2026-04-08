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

    SDL_Window *window = SDL_CreateWindow("Task 103",
                                          WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    bool running = true;
    SDL_Event event;

    // Auto moving circle
    int autoX = 0;
    int autoY = HEIGHT / 3;
    int autoRadius = 30;

    // Player circle
    int playerX = WIDTH / 2;
    int playerY = 0;
    int playerRadius = 30;
    int speed = 5;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    case SDLK_UP: playerY -= speed; break;
                    case SDLK_DOWN: playerY += speed; break;
                    case SDLK_LEFT: playerX -= speed; break;
                    case SDLK_RIGHT: playerX += speed; break;
                }
            }
        }

        autoX += 3;
        if (autoX > WIDTH)
            autoX = 0;

        double distance = sqrt(pow(playerX - autoX, 2) +
                               pow(playerY - autoY, 2));

        bool collision = distance <= (playerRadius + autoRadius);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (collision)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        drawCircle(renderer, autoX, autoY, autoRadius);
        drawCircle(renderer, playerX, playerY, playerRadius);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

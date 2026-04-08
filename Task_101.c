#include <SDL3/SDL.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Task 101",
                                          WIDTH, HEIGHT,
                                          0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    int radius = 100;
    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = 0;
            if(event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                    running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        drawCircle(renderer, centerX, centerY, radius);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
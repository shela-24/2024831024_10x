#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define CELL_SIZE 20

#define GRID_WIDTH (WIDTH / CELL_SIZE)
#define GRID_HEIGHT (HEIGHT / CELL_SIZE)

#define MAX_SNAKE_LENGTH 100

typedef struct
{
    int x;
    int y;
} SnakeSegment;

typedef struct
{
    SnakeSegment body[MAX_SNAKE_LENGTH];

    int length;

    int dirX;
    int dirY;

} Snake;

typedef struct
{
    int x;
    int y;

} Food;

SDL_Window *window = NULL;

SDL_Renderer *renderer = NULL;

TTF_Font *font = NULL;

Snake snake;

Food food;

int score = 0;

bool gameOver = false;

bool initGame(void);

void handleEvents(bool *running);

void updateGame(void);

void renderGame(void);

void closeGame(void);

void initSnake(void);

void updateSnake(void);

void drawSnake(void);

void generateFood(void);

void drawFood(void);

void checkFoodCollision(void);

void checkCollision(void);

void drawText(const char *text, int x, int y);

bool initGame(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow(
        "Snake Game",
        WIDTH,
        HEIGHT,
        0
    );

    if (window == NULL)
    {
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL)
    {
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont("C:/Windows/Fonts/arialbd.ttf", 28);

    if (font == NULL)
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    srand((unsigned int)time(NULL));

    return true;
}

int main(void)
{
    if (!initGame())
    {
        return -1;
    }

    initSnake();

    generateFood();

    bool running = true;

    while (running)
    {
        handleEvents(&running);

        updateGame();

        renderGame();

        SDL_Delay(100);
    }

    closeGame();

    return 0;
}
void initSnake(void)
{
    snake.length = 3;

    snake.body[0].x = 10;
    snake.body[0].y = 10;

    snake.body[1].x = 9;
    snake.body[1].y = 10;

    snake.body[2].x = 8;
    snake.body[2].y = 10;

    snake.dirX = 1;
    snake.dirY = 0;
}

void updateSnake(void)
{
    for(int i = snake.length - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i - 1];
    }

    snake.body[0].x += snake.dirX;
    snake.body[0].y += snake.dirY;
}

void drawSnake(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    for(int i = 0; i < snake.length; i++)
    {
        SDL_FRect rect;

        rect.x = snake.body[i].x * CELL_SIZE;
        rect.y = snake.body[i].y * CELL_SIZE;
        rect.w = CELL_SIZE;
        rect.h = CELL_SIZE;

        SDL_RenderFillRect(renderer, &rect);
    }
}

void generateFood(void)
{
    bool valid;

    do
    {
        valid = true;

        food.x = rand() % GRID_WIDTH;
        food.y = rand() % GRID_HEIGHT;

        for(int i = 0; i < snake.length; i++)
        {
            if(food.x == snake.body[i].x &&
               food.y == snake.body[i].y)
            {
                valid = false;
                break;
            }
        }

    }while(!valid);
}

void drawFood(void)
{
    SDL_FRect rect;

    rect.x = food.x * CELL_SIZE;
    rect.y = food.y * CELL_SIZE;
    rect.w = CELL_SIZE;
    rect.h = CELL_SIZE;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &rect);
}

void handleEvents(bool *running)
{
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_EVENT_QUIT)
        {
            *running = false;
        }

        if(event.type == SDL_EVENT_KEY_DOWN)
        {
            switch(event.key.key)
            {
                case SDLK_UP:

                    if(snake.dirY != 1)
                    {
                        snake.dirX = 0;
                        snake.dirY = -1;
                    }
                    break;

                case SDLK_DOWN:

                    if(snake.dirY != -1)
                    {
                        snake.dirX = 0;
                        snake.dirY = 1;
                    }
                    break;

                case SDLK_LEFT:

                    if(snake.dirX != 1)
                    {
                        snake.dirX = -1;
                        snake.dirY = 0;
                    }
                    break;

                case SDLK_RIGHT:

                    if(snake.dirX != -1)
                    {
                        snake.dirX = 1;
                        snake.dirY = 0;
                    }
                    break;

                case SDLK_R:

                    if(gameOver)
                    {
                        initSnake();

                        generateFood();

                        score = 0;

                        gameOver = false;
                    }
                    break;

                case SDLK_ESCAPE:

                    *running = false;

                    break;
            }
        }
    }
}
void checkFoodCollision(void)
{
    if(snake.body[0].x == food.x &&
       snake.body[0].y == food.y)
    {
        if(snake.length < MAX_SNAKE_LENGTH)
        {
            snake.body[snake.length] =
                snake.body[snake.length - 1];

            snake.length++;
        }

        score++;

        generateFood();
    }
}

void checkCollision(void)
{
    int headX = snake.body[0].x;
    int headY = snake.body[0].y;

    if(headX < 0 || headX >= GRID_WIDTH ||
       headY < 0 || headY >= GRID_HEIGHT)
    {
        gameOver = true;
        return;
    }

    for(int i = 1; i < snake.length; i++)
    {
        if(headX == snake.body[i].x &&
           headY == snake.body[i].y)
        {
            gameOver = true;
            return;
        }
    }
}

void updateGame(void)
{
    if(!gameOver)
    {
        updateSnake();

        checkFoodCollision();

        checkCollision();
    }
}

void drawText(const char *text, int x, int y)
{
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface *surface =
        TTF_RenderText_Blended(font, text, 0, color);

    if(surface == NULL)
    {
        return;
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(renderer, surface);

    if(texture == NULL)
    {
        SDL_DestroySurface(surface);
        return;
    }

    SDL_FRect dst;

    dst.x = x;
    dst.y = y;
    dst.w = (float)surface->w;
    dst.h = (float)surface->h;

    SDL_RenderTexture(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);

    SDL_DestroySurface(surface);
}

void renderGame(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    drawFood();

    drawSnake();

    char scoreText[50];

    sprintf(scoreText, "Score : %d", score);

    drawText(scoreText, 20, 20);

    if(gameOver)
    {
        drawText("GAME OVER", 280, 220);

        char finalScore[50];

        sprintf(finalScore,
                "Final Score : %d",
                score);

        drawText(finalScore, 240, 270);

        drawText("Press R To Restart", 210, 330);

        drawText("ESC To Exit", 260, 380);
    }

    SDL_RenderPresent(renderer);
}

void closeGame(void)
{
    TTF_CloseFont(font);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();
}
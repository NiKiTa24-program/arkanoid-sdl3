#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    float x, y;
    float w, h;
} Rect;

int main() {
    setlocale(LC_ALL, "RU");

    // Инициализация SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow(
        "Arkanoid",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        printf("Ошибка создания окна: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Ошибка создания рендерера: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Rect paddle = {
        (WINDOW_WIDTH - 150) / 2,
        WINDOW_HEIGHT - 50,
        150,
        20
    };

    Rect ball = {
        (WINDOW_WIDTH - 20) / 2,
        paddle.y - 30,
        20,
        20
    };
    float ball_dx = 2.0f;
    float ball_dy = -3.0f;

    SDL_Event event;
    bool running = true;

    while (running) {
        // 1. ОБРАБОТКА СОБЫТИЙ
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                paddle.x = event.motion.x - paddle.w / 2;

                // Ограничение, чтобы платформа не выходила за экран
                if (paddle.x < 0) paddle.x = 0;
                if (paddle.x + paddle.w > WINDOW_WIDTH) {
                    paddle.x = WINDOW_WIDTH - paddle.w;
                }
            }
        }

        // 2. ОБНОВЛЕНИЕ ЛОГИКИ (ВНЕ обработки событий)
        ball.x += ball_dx;
        ball.y += ball_dy;

        // Отскок от левой и правой стен
        if (ball.x < 0 || ball.x + ball.w > WINDOW_WIDTH) {
            ball_dx = -ball_dx;
        }

        // Отскок от верхней стены
        if (ball.y < 0) {
            ball_dy = -ball_dy;
        }

        // Проверка проигрыша (мяч упал вниз)
        if (ball.y + ball.h > WINDOW_HEIGHT) {
            printf("Game Over!\n");
            running = false;
        }

        // Отскок от платформы
        if (ball_dy > 0 &&
            ball.x < paddle.x + paddle.w &&
            ball.x + ball.w > paddle.x &&
            ball.y + ball.h >= paddle.y &&
            ball.y + ball.h <= paddle.y + paddle.h) {
            ball_dy = -ball_dy;
            ball.y = paddle.y - ball.h;
        }

        // 3. РИСОВАНИЕ
        // Очистка экрана (чёрный фон)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Рисуем платформу
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect rect = { paddle.x, paddle.y, paddle.w, paddle.h };
        SDL_RenderFillRect(renderer, &rect);

        // Рисуем мяч
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_FRect ballRect = { ball.x, ball.y, ball.w, ball.h };
        SDL_RenderFillRect(renderer, &ballRect);

        // Показываем на экране
        SDL_RenderPresent(renderer);
    }

    // Очистка
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
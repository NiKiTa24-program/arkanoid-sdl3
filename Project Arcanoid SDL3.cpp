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

    // Инициализация SDL (ОДИН РАЗ!)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Ошибка инициализации SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow(
        "Arkanoid",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE   // ← было REZIABLE, теперь RESIZABLE
    );
    if (!window) {
        printf("Ошибка создания окна: %s\n", SDL_GetError());  // ← нужны скобки ()
        SDL_Quit();
        return 1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);  // ← было NILL, теперь NULL
    if (!renderer) {
        printf("Ошибка создания рендерера: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // ---- РИСОВАНИЕ ----
        // Здесь будем рисовать платформу и мяч

        SDL_RenderPresent(renderer);
    }

    // Очистка (передаём объекты!)
    SDL_DestroyRenderer(renderer);  // ← нужен renderer
    SDL_DestroyWindow(window);      // ← нужен window
    SDL_Quit();

    return 0;
}
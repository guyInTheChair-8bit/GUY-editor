#include <SDL2/SDL.h>
#include <stdio.h>

const char* TITLE = "GUY-EDITOR";
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1920;

void toggle_fullscreen (SDL_Window* window, int is_fullscreen);

int main (int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (window == NULL) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Event event;
    int is_running = 1;
    int is_fullscreen = 0;

    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) is_running = 0;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_f) { 
                    is_fullscreen = !is_fullscreen;
                    toggle_fullscreen(window, is_fullscreen);
                }
                if (event.key.keysym.sym == SDLK_ESCAPE) is_running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
} 

void toggle_fullscreen (SDL_Window* window, int is_fullscreen) {
    if (is_fullscreen)  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    else SDL_SetWindowFullscreen(window, 0);
}

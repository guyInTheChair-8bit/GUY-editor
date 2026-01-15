#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>

const char* TITLE = "GUY-EDITOR";
int winWidth = 1920;
int winHeight = 1920;
const int ptsize = 48;

void toggle_fullscreen (SDL_Window* window, int is_fullscreen);
void drawText(const char* text, int x, int y, TTF_Font* font, SDL_Color* color, SDL_Renderer* renderer, int winWidth);

int main (int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF initialization error: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, 0);

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

    SDL_StartTextInput();

    TTF_Font* font = TTF_OpenFont("font.ttf", ptsize);
    SDL_Color white = {255, 255, 255, 255};

    SDL_Event event;
    int is_running = 1;
    int is_fullscreen = 0;

    char strBuffer[1024] = "";

    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) is_running = 0;
            else if (event.type == SDL_TEXTINPUT) {
                if (strlen(strBuffer) + strlen(event.text.text) < 1024) {
                    strcat(strBuffer, event.text.text);
                } 
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_f) { 
                    if (event.key.keysym.mod & KMOD_CTRL || event.key.keysym.mod & KMOD_GUI) {
                        is_fullscreen = !is_fullscreen;
                        toggle_fullscreen(window, is_fullscreen);
                    }
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE) strBuffer[strlen(strBuffer) - 1] = '\0';
                else if (event.key.keysym.sym == SDLK_RETURN) strBuffer[strlen(strBuffer)] = '\n';
                else if (event.key.keysym.sym == SDLK_ESCAPE && is_fullscreen) {
                    is_fullscreen = !is_fullscreen;
                    toggle_fullscreen(window, 0);
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_GetWindowSize(window, &winWidth, &winHeight);
        drawText(strBuffer, 100, 100, font, &white, renderer, winWidth);
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

void drawText(const char* text, int x, int y, TTF_Font* font, SDL_Color* color, SDL_Renderer* renderer, int winWidth) {

    SDL_Surface* surfaceMsg = TTF_RenderText_Blended_Wrapped(font, text, *color, winWidth - 200);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surfaceMsg);
    SDL_FreeSurface(surfaceMsg);
    
    int texW;
    int texH;

    SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);
    
    SDL_Rect dstrect;
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = texW;
    dstrect.h = texH;

    SDL_RenderCopy(renderer, tex, NULL, &dstrect);
    SDL_DestroyTexture(tex);
}

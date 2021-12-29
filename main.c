
#include <stdio.h>
#include <SDL.h>
//#include <SDL2/SDL_image.h>

int width = 400;
int height = 300;

int framrate = 60;

int fps_o = 5;
int fps = 0;
int x = 0;

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;

void init(){

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        exit(-1);}

    return 0;}

void blit(SDL_Surface* surface, SDL_Window* window){

    if (x <= 0) {
        fps = fps_o;
    }
        
    if (x >= 300){
        fps = -fps_o;
    }

    x += fps;

    SDL_Rect background = { 0, 0, width, height };
    SDL_FillRect(surface, &background, 0xffffffff);// ARGB

    struct SDL_Rect redRect = { x, 0, 100, 100 };
    SDL_FillRect(surface, &redRect, 0xffff0000);

    SDL_UpdateWindowSurface(window);}

void event(SDL_Surface* surface, SDL_Window* window) {

    while (1) {

        uint32_t begin = SDL_GetTicks();

        blit(surface, window);

        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {

                //SDL_Quit();

                return;
            }
        }

        long current = SDL_GetTicks();
        long cost = current - begin;
        long frame = 1000 / framrate;
        long delay = frame - cost;

        if (delay > 0) {
            SDL_Delay(delay);
        }
    }
}

int main(){

    init();

    window = SDL_CreateWindow("MoYu Engine C",
                                SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                                width, height,
                                SDL_WINDOW_SHOWN);

    surface = SDL_GetWindowSurface(window);

    SDL_UpdateWindowSurface(window); \

    event(surface, window);

    SDL_DestroyWindow(window);

    return 0;}

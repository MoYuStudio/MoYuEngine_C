
#include <stdio.h>
#include <SDL.h>

int width = 400;
int height = 300;

int x = 0;

void blit(SDL_Surface* screen, SDL_Window* win) {

    x++;

    if (x == 300){
        x = 0;}

    SDL_Rect r = { 0, 0, width, height };
    SDL_FillRect(screen, &r, 0xffffffff);// ARGB
    struct SDL_Rect redRect = { x, 0, 100, 100 };
    SDL_FillRect(screen, &redRect, 0xffff0000);
    SDL_UpdateWindowSurface(win);}

void event(SDL_Surface* screen, SDL_Window* win){

    while (1){
        blit(screen, win);

        SDL_Event event;
        if (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                break;}}}}

int main(){

    SDL_Window* win = SDL_CreateWindow("SUGT12",
                                        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                                        width, height,
                                        SDL_WINDOW_SHOWN);

    SDL_Surface* screen = SDL_GetWindowSurface(win);

    SDL_UpdateWindowSurface(win);\

    event(screen, win);
    SDL_DestroyWindow(win);

    return 0;}

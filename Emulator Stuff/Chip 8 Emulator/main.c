#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

//cd "Emulator Stuff/Chip 8 Emulator"
void windowing();

int main(int argc, char* argv[]) {
    windowing();
    return 1;
}


void windowing() {
    if (!SDL_Init(SDL_INIT_VIDEO)){
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("Chip 8 Emulator", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
    }
}
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

//cd "C:\Users\Stephen\Documents\GitHub\Emulators\Chip 8 Emulator"
void windowing();

typedef struct{
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint16_t stack[16];
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint8_t keypad[16];
    uint8_t display[SCREEN_WIDTH * SCREEN_HEIGHT];
} Chip8;




int main(int argc, char* argv[]){
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
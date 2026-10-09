#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

//  cd "C:\Users\Stephen\Documents\GitHub\Emulators\Chip 8 Emulator"     // change directory to the project folder
// C:\Users\Stephen\Documents\GitHub\Emulators\ROMs\Airplane.ch8        // location of ROM


typedef struct chip8_t chip8_t;
bool SDL_start(chip8_t* chip);
bool chip_init(chip8_t* chip, const char *rom_file_path);

typedef struct instructions_t{
    uint16_t opcode;
    uint16_t NNN; // 12 bit address/counter
    uint8_t NN; // 8 bit immediate number
    uint8_t N; // 4 bit number
    uint8_t X; 
    uint8_t Y;
} instructions_t;

typedef struct chip8_t {
    uint8_t memory[4096]; //Allocate memory
    uint8_t V[16]; //Allocate registers
    uint16_t I; //Allocate index register
    uint16_t PC; //Allocate program counter
    uint16_t stack[16]; 
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint8_t keypad[16];
    uint8_t display[SCREEN_WIDTH * SCREEN_HEIGHT];
    instructions_t instruction;
    bool running;
    bool isPaused;
} chip8_t;




int main(int argc, char *argv[]){
    if (argc<2){
        printf("Usage: %s <ROM file>\n", argv[0]);
        return 1;
    }
    chip8_t chip = {0};
    if (!chip_init(&chip, argv[1])) {
        return 1;
    }


    SDL_start(&chip);
    return 0;
}


void emulate_instruction(chip8_t* chip) {
    chip->instruction.opcode = (chip->memory[chip->PC] << 8) | chip->memory[chip->PC + 1];
}


bool chip_init(chip8_t* chip, const char *rom_file_path) {
    const uint8_t Font[80] ={
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    memcpy(&chip->memory[0x050], Font, sizeof(Font));
    chip->PC = 0x200; // Program counter starts at 0x200
    chip->I = 0;      // Reset index register
    chip->sound_timer = 0;
    chip->delay_timer = 0;
    FILE *rom_file = fopen(rom_file_path, "rb");
    if (!rom_file) {
        printf("Failed to open ROM file: %s\n", rom_file_path);
        return false;
    }

    fseek(rom_file, 0, SEEK_END);
    long rom_size = ftell(rom_file);
    rewind(rom_file);

    if (rom_size > (4096 - 512)) {
        printf("ROM file is too large to fit in memory\n");
        fclose(rom_file);
        return false;
    }
    fread(&chip->memory[0x200], rom_size, 1, rom_file);

    fclose(rom_file);

    return true;
}



bool SDL_start(chip8_t* chip) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_Window *window = SDL_CreateWindow("Chip 8 Emulator", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0)) {
        printf("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    chip-> running = true;
    chip -> isPaused = false;
    SDL_Event event;

    while (chip->running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_EVENT_QUIT:
                chip->running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.repeat){
                    break;
                }
            switch (event.key.key){
                case SDLK_ESCAPE:
                    chip->running = false;
                    break;
                case SDLK_SPACE:
                    printf("Emulator paused. Press 'SPACE' to resume.\n");
                    printf("%s", chip->isPaused ? "Resuming..." : "Pausing...");
                    chip->isPaused = !chip->isPaused;
                    break;
            }
            break;
            }
        }

        if (chip->isPaused) {
            SDL_Delay(16);
            continue;
        }

        if (!SDL_RenderClear(renderer)) {
            printf("SDL_RenderClear failed: %s\n", SDL_GetError());
            chip->running = false;
            break;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return true;
}

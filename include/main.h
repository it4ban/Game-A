#pragma once

#define GAME_NAME "GameA"
#define K_SCREEN_WIDTH 640
#define K_SCREEN_HEIGHT 480

bool gameIsRunning = false;

bool createMainGameWindow(SDL_Window** window, SDL_Surface** surface);

void processPlayerInput(void);

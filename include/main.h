#pragma once

#define GAME_NAME "GameA"
#define K_SCREEN_WIDTH 640
#define K_SCREEN_HEIGHT 480

bool gameIsRunning = false;

typedef struct Game
{
    SDL_Window* pWindow;
    SDL_Surface* pSurface;
} Game;

bool createMainGameWindow(Game* game);

bool processPlayerInput(Game* game);

bool renderFrameGraphics(Game* game);

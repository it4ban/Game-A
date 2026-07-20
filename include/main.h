#pragma once

#define GAME_NAME "GameA"
#define K_SCREEN_WIDTH 640
#define K_SCREEN_HEIGHT 480

bool createMainGameWindow(SDL_Window** window, SDL_Surface** surface);

static void cleanup(SDL_Window** window, SDL_Surface** surface);

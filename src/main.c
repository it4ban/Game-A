#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "main.h"
#include "single_instance.h"

static void cleanup(Game* game);

int main(int argc, char* argv[])
{
    SingleInstanceResult programStatus = singleInstanceAcquire();
    if (programStatus == SINGLE_INSTANCE_ALREADY_RUNNING)
    {
        return 0;
    }

    if (programStatus == SINGLE_INSTANCE_ERROR)
    {
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        singleInstanceRelease();
        return 1;
    }

    Game game = {.pWindow = NULL, .pSurface = NULL};

    if (createMainGameWindow(&game) == false)
    {
        cleanup(&game);
        return 1;
    }

    gameIsRunning = true;

    while (gameIsRunning)
    {
        gameIsRunning = processPlayerInput(&game);
        if (!gameIsRunning)
        {
            break;
        }

        SDL_FillSurfaceRect(game.pSurface, NULL, SDL_MapSurfaceRGB(game.pSurface, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(game.pWindow);

        SDL_Delay(16);
    }

    cleanup(&game);
    return 0;
}

bool createMainGameWindow(Game* game)
{
    if (game == NULL)
    {
        SDL_Log("Invalid output pointers");
        return false;
    }

    game->pWindow = SDL_CreateWindow(GAME_NAME, K_SCREEN_WIDTH, K_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (game->pWindow == NULL)
    {
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        return false;
    }

    game->pSurface = SDL_GetWindowSurface(game->pWindow);
    if (game->pSurface == NULL)
    {
        SDL_Log("Could not get window surface: %s\n", SDL_GetError());
        cleanup(game);
        return false;
    }

    return true;
}

bool processPlayerInput(Game* game)
{
    SDL_Event event;
    SDL_zero(event);

    while (SDL_PollEvent(&event) == true)
    {
        switch (event.type)
        {
        case SDL_EVENT_WINDOW_RESIZED:
            game->pSurface = SDL_GetWindowSurface(game->pWindow);
            if (game->pSurface == NULL)
            {
                SDL_Log("Could not update window surface: %s", SDL_GetError());

                return false;
            }

            break;
        case SDL_EVENT_QUIT:
            return false;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.repeat)
                break;

            switch (event.key.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                return false;
                break;
            default:
                break;
            }
        }
    }

    return true;
}

bool renderFrameGraphics(Game* game);

static void cleanup(Game* game)
{
    if (game != NULL && game->pWindow != NULL)
    {
        SDL_DestroyWindow(game->pWindow);
        game->pWindow = NULL;
        game->pSurface = NULL;
    }

    singleInstanceRelease();

    SDL_Quit();
}

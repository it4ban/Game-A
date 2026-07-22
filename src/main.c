#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "main.h"
#include "single_instance.h"

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

    SDL_Window* pWindow = NULL;
    SDL_Surface* pScreenSurface = NULL;

    if (createMainGameWindow(&pWindow, &pScreenSurface) == false)
    {
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event event;
    SDL_zero(event);

    while (quit == false)
    {
        while (SDL_PollEvent(&event) == true)
        {
            switch (event.type)
            {
            case SDL_EVENT_WINDOW_RESIZED:
                pScreenSurface = SDL_GetWindowSurface(pWindow);
                break;
            case SDL_EVENT_QUIT:
                quit = true;
                break;
            }
        }

        SDL_FillSurfaceRect(pScreenSurface, NULL, SDL_MapSurfaceRGB(pScreenSurface, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(pWindow);
    }

    cleanup(&pWindow, &pScreenSurface);
    return 0;
}

bool createMainGameWindow(SDL_Window** window, SDL_Surface** surface)
{
    if (window == NULL || surface == NULL)
    {
        SDL_Log("Invalid output pointers");
        return false;
    }

    *window = SDL_CreateWindow(GAME_NAME, K_SCREEN_WIDTH, K_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (*window == NULL)
    {
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        return false;
    }

    *surface = SDL_GetWindowSurface(*window);
    if (*surface == NULL)
    {
        SDL_Log("Could not get window surface: %s\n", SDL_GetError());
        cleanup(window, surface);
        return false;
    }

    return true;
}

static void cleanup(SDL_Window** window, SDL_Surface** surface)
{
    if (window != NULL && *window != NULL)
    {
        SDL_DestroyWindow(*window);
        *window = NULL;
    }

    if (surface != NULL)
    {
        *surface = NULL;
    }

    singleInstanceRelease();

    SDL_Quit();
}

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define GAME_NAME "GameA"
#define K_SCREEN_WIDTH 640
#define K_SCREEN_HEIGHT 480

static void cleanup(SDL_Window** window, SDL_Surface** surface);

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* pWindow = NULL;
    SDL_Surface* pScreenSurface = NULL;

    pWindow = SDL_CreateWindow(GAME_NAME, K_SCREEN_WIDTH, K_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (pWindow == NULL)
    {
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    pScreenSurface = SDL_GetWindowSurface(pWindow);
    if (pScreenSurface == NULL)
    {
        SDL_Log("Could not get window surface: %s\n", SDL_GetError());
        cleanup(&pWindow, &pScreenSurface);
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

    SDL_Quit();
}

/*
 * periphs.cpp
 *
 * Travis Banken
 * 2020
 *
 * Peripherals. Handles drawing to the window and
 * key events.
 */

#include <cstdlib>
#include <iostream>
#include <periphs.h>

#define SCALED(x) (16*x)

#define FRAME_HEIGHT 32
#define FRAME_WIDTH 64
#define WINDOW_HEIGHT SCALED(FRAME_HEIGHT)
#define WINDOW_WIDTH SCALED(FRAME_WIDTH)


Periphs::Periphs(const char *title)
{
    int rc;
    // init sdl
    rc = SDL_Init(SDL_INIT_VIDEO);
    if (rc < 0) {
        std::cerr << "Error: SDL_Init: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // create window
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == NULL) {
        std::cerr << "Error: SDL_CreateWindow: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // get surface
    m_surface = SDL_GetWindowSurface(m_window);

    SDL_FillRect(m_surface, NULL, SDL_MapRGB(m_surface->format, 0x00, 0x00, 0x00));
    SDL_UpdateWindowSurface(m_window);
    SDL_Delay(10000);
}

Periphs::~Periphs()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
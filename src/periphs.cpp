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

#define FRAME_HEIGHT 32
#define FRAME_WIDTH 64

Periphs::Periphs(const char *title, uint pxscale)
    : m_pxscale(pxscale), m_framebuf(FRAME_HEIGHT*FRAME_WIDTH)
{
    int rc;

    std::fill(m_framebuf.begin(), m_framebuf.end(), 0);


    // init sdl
    rc = SDL_Init(SDL_INIT_VIDEO);
    if (rc < 0) {
        std::cerr << "Error: SDL_Init: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    uint wh = scale(FRAME_HEIGHT);
    uint ww = scale(FRAME_WIDTH);
    // create window
    m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                ww, wh, SDL_WINDOW_SHOWN);
    if (m_window == NULL) {
        std::cerr << "Error: SDL_CreateWindow: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if (m_renderer == NULL) {
        std::cerr << "Error: SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        std::exit(1);
    }

    // test
    // for (int i = 0; i < 64; i++) {
    //     for (int j = 0; j < 32; j++) {
    //         place_pixel(i, j, 1);
    //         refresh();
    //         SDL_Delay(90);
    //     }
    // }
    // while (1)
    //     poll();
}

Periphs::~Periphs()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}

void Periphs::clear_screen()
{
    // call clear screen
    SDL_RenderClear(m_renderer);

    // fill screen black
    SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_Rect rectangle;
    rectangle.x = 0;
    rectangle.y = 0;
    rectangle.w = scale(FRAME_WIDTH);
    rectangle.h = scale(FRAME_HEIGHT);
    SDL_RenderFillRect(m_renderer, &rectangle);
    // clear buf
    std::fill(m_framebuf.begin(), m_framebuf.end(), 0);
    refresh();
}

bool Periphs::place_pixel(uint8_t x, uint8_t y, uint8_t pixval)
{
    uint pos = x*FRAME_HEIGHT + y;

    // don't draw outside of range
    if (pos >= m_framebuf.size()) {
        std::cerr << "Warning: Tried to draw outside of screen range!\n";
        return false;
    }

    bool collision = m_framebuf[pos] && pixval;
    uint8_t col = m_framebuf[pos] ^ pixval ? 0xFF : 0x00;

    // update buffer
    m_framebuf[pos] = col ? 1 : 0;

    SDL_SetRenderDrawColor(m_renderer, col, col, col, SDL_ALPHA_OPAQUE);
    SDL_Rect rectangle;

    rectangle.x = scale(x);
    rectangle.y = scale(y);
    rectangle.w = scale(1);
    rectangle.h = scale(1);
    SDL_RenderFillRect(m_renderer, &rectangle);
    return collision;
}

void Periphs::refresh()
{
    SDL_RenderPresent(m_renderer);
    poll();
}

void Periphs::poll()
{
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            std::exit(0);
        }
    }
}

uint Periphs::scale(uint x)
{
    return x * m_pxscale;
}
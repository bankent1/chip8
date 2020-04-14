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
    : m_pxscale(pxscale), m_framebuf(FRAME_HEIGHT*FRAME_WIDTH), m_timer(0)
{
    int rc;

    // init clock
    m_last_tick = std::chrono::high_resolution_clock::now();

    // init keymap
    m_keymap[SDLK_0] = 0;
    m_keymap[SDLK_1] = 1;
    m_keymap[SDLK_2] = 2;
    m_keymap[SDLK_3] = 3;
    m_keymap[SDLK_4] = 4;
    m_keymap[SDLK_5] = 5;
    m_keymap[SDLK_6] = 6;
    m_keymap[SDLK_7] = 7;
    m_keymap[SDLK_8] = 8;
    m_keymap[SDLK_9] = 9;
    // QWERTY style mapping
    m_keymap[SDLK_q] = 10;
    m_keymap[SDLK_w] = 11;
    m_keymap[SDLK_e] = 12;
    m_keymap[SDLK_r] = 13;
    m_keymap[SDLK_t] = 14;
    m_keymap[SDLK_y] = 15;
    // ABCDEF style mapping
    // m_keymap[SDLK_a] = 10;
    // m_keymap[SDLK_b] = 11;
    // m_keymap[SDLK_c] = 12;
    // m_keymap[SDLK_d] = 13;
    // m_keymap[SDLK_e] = 14;
    // m_keymap[SDLK_f] = 15;

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
    // for (int t = 0; t < 2; t++)
    // for (int i = 0; i < 64; i++) {
    //     for (int j = 0; j < 32; j++) {
    //         place_pixel(i, j, 1);
    //         refresh();
    //         SDL_Delay(10);
    //     }
    // }
    // while (1)
    //     std::cerr << "Keypress: " << (uint)await_keypress() << std::endl; 
    // while (1)
    //     poll_quit();
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
    y = y % FRAME_HEIGHT;
    x = x % FRAME_WIDTH;
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
    poll_quit();
    SDL_RenderPresent(m_renderer);
    update_timer();
    // some primative timing trick
    m_clock++;
    if (m_clock > 10) {
        SDL_Delay(16);
        m_clock = 0;
    }
}

void Periphs::poll_quit()
{
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            std::exit(0);
        }
    }
}

uint8_t Periphs::await_keypress()
{
    uint8_t res;
    while (true) {
        res = get_keystate();
        if (res != NO_KEY)
            return res;
    }
    return NO_KEY;
}

uint8_t Periphs::get_keystate()
{
    SDL_Event e;
    SDL_Keycode keycode;
    if (SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_QUIT:
            std::exit(0);
            break;
        case SDL_KEYDOWN:
            keycode = e.key.keysym.sym;
            auto it = m_keymap.find(keycode);
            if (it != m_keymap.end()) {
                return m_keymap[keycode];
            }
            break;
        }
    }
    return NO_KEY;
}

void Periphs::update_timer()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - m_last_tick;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    if (microseconds >= (long long)(1000000 / 60)) {
        m_timer -= m_timer == 0 ? 0 : 1;
        m_last_tick = std::chrono::high_resolution_clock::now();
    }
    // if (m_timer > 0) {
    //     SDL_Delay(16);
    //     m_timer--;
    // }
}

void Periphs::set_timer(uint8_t ticks)
{
    m_timer = ticks;
}

uint8_t Periphs::get_timer()
{
    return m_timer;
}

uint Periphs::scale(uint x)
{
    return x * m_pxscale;
}
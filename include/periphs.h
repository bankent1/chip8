#ifndef _PERIPHS_H
#define _PERIPHS_H

#include <SDL.h>
#include <cstdint>
#include <string>


class Periphs {
private:
	SDL_Window *m_window;
	SDL_Surface *m_surface;

public:
	Periphs(const char *title);
	~Periphs();
	// void clear_screen();
	// void place_pixel(uint8_t r, uint8_t g, uint8_t b);
};

#endif
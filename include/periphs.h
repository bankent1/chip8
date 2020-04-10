#ifndef _PERIPHS_H
#define _PERIPHS_H

#include <SDL.h>
#include <cstdint>
#include <string>
#include <vector>


class Periphs {
private:
	SDL_Window *m_window;
	// SDL_Surface *m_surface;
	SDL_Renderer *m_renderer;
	uint m_pxscale;
	std::vector<uint8_t> m_framebuf;

	uint scale(uint x);

public:
	Periphs(const char *title, uint pxscale);
	~Periphs();
	void clear_screen();
	bool place_pixel(uint8_t x, uint8_t y, uint8_t pixval);
	void poll();
	void refresh();
};

#endif
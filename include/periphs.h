#ifndef _PERIPHS_H
#define _PERIPHS_H

#include <SDL.h>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#define NO_KEY 0xF0


class Periphs {
private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	uint m_pxscale;
	std::vector<uint8_t> m_framebuf;
	std::map<SDL_Keycode, uint8_t> m_keymap;

	uint scale(uint x);

public:
	Periphs(const char *title, uint pxscale);
	~Periphs();
	void clear_screen();
	bool place_pixel(uint8_t x, uint8_t y, uint8_t pixval);
	void poll();
	uint8_t await_keypress();
	uint8_t get_keystate();
	void refresh();
};

#endif
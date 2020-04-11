#ifndef _PERIPHS_H
#define _PERIPHS_H

#include <SDL.h>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <chrono>

#define NO_KEY 0xF0


class Periphs {
private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	uint m_pxscale;
	std::vector<uint8_t> m_framebuf;
	std::map<SDL_Keycode, uint8_t> m_keymap;
	std::chrono::high_resolution_clock::time_point m_last_tick;
	uint8_t m_timer;


	uint scale(uint x);
	void poll_quit();
	void update_timer();

public:
	Periphs(const char *title, uint pxscale);
	~Periphs();
	void clear_screen();
	bool place_pixel(uint8_t x, uint8_t y, uint8_t pixval);
	uint8_t await_keypress();
	uint8_t get_keystate();
	void refresh();
	void set_timer(uint8_t ticks);
	uint8_t get_timer();
};

#endif
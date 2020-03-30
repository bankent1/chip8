/*
 * graphics.c
 *
 * Author: Travis Banken
 *
 * The graphics framework for the chip8 processor.
 */

#include <stdint.h>
#include <ncurses.h>

#include "chip8.h"
#include "graphics.h"


// TODO:
// 	- Init
//	- Shutdown
// 	- draw row
// 	- clear row


static int initialized = 0;
static char PIXEL_ON = '*';

/*
 * Initializes the graphics framework for chip8
 */
int graphics_init()
{
	LOG("Initializing graphics with size %d x %d\n", GRAPH_ROWS, GRAPH_COLS);
	int result = CHIP8_SUCCESS;

	if (initialized) {
		PRINT_ERROR("Graphics already initialized!\n");
		return CHIP8_ERROR;
	}

	// init ncurses
	initscr();
//	noecho();
//	cbreak();


	initialized = 1;
	return result;
}

/*
 * Shutdown graphics framework and free all resources
 */
int graphics_shutdown()
{
	LOG("Shutting down graphics\n");
	int result = CHIP8_SUCCESS;

	if (!initialized) {
		PRINT_ERROR("Graphics never initialized!\n");
		return CHIP8_ERROR;
	}

	initialized = 0;
	// end ncurses
	endwin();

	return result;
}

/*
 *
 */
int drawfb(uint8_t *(frame_buf[GRAPH_ROWS][GRAPH_COLS]))
{
	LOG("Refreshing screen\n");

	for (int row = 0; row < GRAPH_ROWS; row++) {
		for (int col = 0; col < GRAPH_COLS; col++) {
			// iterate over byte
			uint8_t byte = *(frame_buf[row][col]);
			for (int i = 1; i < 8; i++) {
				// draw to screen on 1 bits
				if ((byte & 0x1) == 1) {
					mvaddch(row, col, (chtype) PIXEL_ON);
				}
				byte = byte >> i;
			}
		}
	}
	return CHIP8_SUCCESS;
}

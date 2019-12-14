/*
 * graphics.c
 *
 * Author: Travis Banken
 *
 * The graphics framework for the chip8 processor.
 */
#include <stdint.h>


#include "chip8.h"
#include "graphics.h"

// TODO:
// 	- Init
//	- Shutdown
// 	- draw row
// 	- clear row

/*
 * Initializes the graphics framework for chip8
 */
int graphics_init()
{
	LOG("Initializing graphics with size %d x %d\n", GRAPH_ROWS, GRAPH_COLS);
	int result = CHIP8_SUCCESS;

	return result;
}

/*
 * Shutdown graphics framework and free all resources
 */
int graphics_shutdown()
{
	LOG("Shutting down graphics\n");
	int result = CHIP8_SUCCESS;

	return result;
}

/*
 * 
 */
int draw_row(int row, int *contents)
{
	int result = CHIP8_SUCCESS;

	return result;
}

/*
 *
 */
int clear_row(int row)
{
	int result = CHIP8_SUCCESS;

	return result;
}
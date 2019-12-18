/*
 * graphics.h
 *
 * Author: Travis Banken
 *
 * Header file for the graphics framework
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

// 64x32 display
#define GRAPH_ROWS 64
#define GRAPH_COLS 32

int graphics_init();
int graphics_shutdown();
int drawfb();


#endif

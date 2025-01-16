#pragma once

#include "types.h"

#define TAB_STOP_SIZE (4)

// unsigned integer size 8 bits (0-255)
//typedef unsigned char uint8;

// yes, assignments are unnecessary, but provided for completeness
typedef enum text_color {
	BLACK 		= 0x0,
	BLUE 		= 0x1,
	GREEN 		= 0x2,
	CYAN 		= 0x3,
	RED 		= 0x4,
	MAGENTA 	= 0x5,
	BROWN 		= 0x6,
	LT_GRAY 	= 0x7,
	DK_GRAY 	= 0x8,
	LT_BLUE 	= 0x9,
	LT_GREEN 	= 0xa,
	LT_CYAN 	= 0xb,
	LT_RED 		= 0xc,
	PINK 		= 0xd,
	YELLOW		= 0xe,
	WHITE 		= 0xf
} text_color;

typedef struct {
	const uint8_t x;
	const uint8_t y;
} t_screen_size;

typedef struct {
	uint8_t x;
	uint8_t y;
} t_cursor_coord;

/* set the color for the next output */
void set_color(enum text_color background, enum text_color foreground);

/* clear screen */
void cls(void);

/* print a null-terminated string */
int puts(const char* const s);

/* print a single character to the current cursor location */
int putch(const char c);
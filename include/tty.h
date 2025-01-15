#pragma once

#define TAB_STOP_SIZE (4)

// unsigned integer size 8 bits (0-255)
typedef unsigned char uint8;

typedef struct {
	const uint8 x;
	const uint8 y;
} t_screen_size;

typedef struct {
	uint8 x;
	uint8 y;
} t_cursor_coord;

/* clear screen */
void cls(void);

/* print a null-terminated string */
int puts(const char* const s);

/* print a single character to the current cursor location */
int putch(const char c);
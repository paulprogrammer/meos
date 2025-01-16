#define VGA_ROWS_NUM 25
#define VGA_COLUMNS_NUM 80

#define ARRAY_SIZE(arr) ((int) sizeof(arr) / (int) sizeof ((arr)[0]))

#define CHAR_LOCATION(x,y) (((int) x*2) + (y*(VGA_COLUMNS_NUM * 2)))
#define COLOR_LOCATION(x,y) ((((int) x*2) + (y*(VGA_COLUMNS_NUM * 2))+1))

#define COMPOSE_COLOR(fgnd,bkgnd) ((uint8_t) ((text_color) fgnd)<<4 | ((text_color) bkgnd))

#include "tty.h"
#include "memops.h"

volatile uint8_t *vga_buf = (uint8_t *)0xb8000;
const uint8_t cursor='\xf9';

// default = lt gray on black
uint8_t color = 0x07;

t_cursor_coord current_pos = {
	.x = 0,
	.y = 0
};

void set_color(text_color background, text_color foreground) {
	color = COMPOSE_COLOR(foreground, background);
}

void advance_cursor() {
	current_pos.x++;
	if( current_pos.x >= VGA_COLUMNS_NUM) {
		current_pos.y++;
		current_pos.x=0;
	}
}

void hide_cursor(void) {
	vga_buf[CHAR_LOCATION(current_pos.x,current_pos.y)]=' ';
}

void show_cursor(void) {
	vga_buf[CHAR_LOCATION(current_pos.x,current_pos.y)]=cursor;
	vga_buf[COLOR_LOCATION(current_pos.x,current_pos.y)]=color;
}

void cls(void) {
	hide_cursor();
	bzero(vga_buf, VGA_COLUMNS_NUM * VGA_ROWS_NUM * 2);
	bzero(&current_pos, sizeof(current_pos));
	show_cursor();
}

void _carrage_return() {
	hide_cursor();
	current_pos.x = 0;
	current_pos.y++;
	show_cursor();
	// TODO: scroll up when we hit the bottom of the buffer.
}

void _putch_tab() {
	hide_cursor();
	// figure out the next tab stop
	uint8_t tab = (TAB_STOP_SIZE - (current_pos.x % TAB_STOP_SIZE));
	// tab now holds the number of spaces needed to get the next tab column
	// however, if the tab extends beyond the end of the visibile screen, 
	// just advance to the next row.
	if( (current_pos.x + tab) > VGA_COLUMNS_NUM ) {
		_carrage_return();
	} else {
		for( int i = 0; i < tab; i++) {
			putch(' ');
		}
	}
}

int putch(const char c) {
	switch(c) {
	case 0 ... 0x08:
	case 0x0b ... 0x1f:
	case 0x7f:
		// control chars, excepting \n and \t which we define below.
		// TODO: 
		//  0x08 (backspace): delete the character before the cursor, decrement location
		//  0x7f (del): delete the character before the cursor, decrement location
		//	0x07 (\b, bell): ding, flash screen
		break;

	case '\n':
		_carrage_return();
		break;

	case '\t': 
		_putch_tab();
		break;
	
	default:
		vga_buf[CHAR_LOCATION(current_pos.x,current_pos.y)]=c;
		vga_buf[COLOR_LOCATION(current_pos.x,current_pos.y)]=color;
		advance_cursor();
		show_cursor();
		break;
	}
	return 1;
}

int puts(const char* const s) {
	int output = 0;
	for( int i=0; s[i] != '\0'; i++) {
		output += putch(s[i]);
	}
	return output;
}

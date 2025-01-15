#define VGA_ROWS_NUM 25
#define VGA_COLUMNS_NUM 80

#define ARRAY_SIZE(arr) ((int) sizeof(arr) / (int) sizeof ((arr)[0]))

#define CHAR_LOCATION(x,y) (((int) x*2) + (y*(VGA_COLUMNS_NUM * 2)))
#define COLOR_LOCATION(x,y) ((((int) x*2) + (y*(VGA_COLUMNS_NUM * 2))+1))

#include "tty.h"
#include "memops.h"

volatile uint8 *vga_buf = (uint8 *)0xb8000;
const uint8 cursor='\xf9';

uint8 color = 0x07;

t_cursor_coord current_pos = {
	.x = 0,
	.y = 0
};

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
	uint8 tab = (TAB_STOP_SIZE - (current_pos.x % TAB_STOP_SIZE));
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
	if( c == '\t') {
		_putch_tab();
		return 1;
	}

	vga_buf[CHAR_LOCATION(current_pos.x,current_pos.y)]=c;
	advance_cursor();
	show_cursor();
	return 1;
}

int puts(const char* const s) {
	int output = 0;
	for( int i=0; s[i] != '\0'; i++) {
		const char c = s[i];
		switch(c) {
		case '\n':
			_carrage_return();
			output ++;
			break;

		default:
			output += putch(s[i]);
		}		
	}
	return output;
}
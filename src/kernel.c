#define VGA_COLUMNS_NUM 80
#define VGA_ROWS_NUM 25

#define ARRAY_SIZE(arr) ((int) sizeof(arr) / (int) sizeof ((arr)[0]))

void _start_kernel(void) {
	volatile char *vga_buf = (char *)0xb8000;
	const char msg[] = "loading kernel";
	
	/* a stupid bzero() routine to clear the VGA buffer */
	for( int i=0; i < VGA_COLUMNS_NUM * VGA_ROWS_NUM * 2; i++) {
		vga_buf[i] = 0;
	}

	/* writes a line in the vga buffer directly */
	for( int i=0; i < ARRAY_SIZE(msg) - 1; i++) {
		vga_buf[i*2] = msg[i];
		vga_buf[i*2+1] = 0x07; /* white on black */
	}

}
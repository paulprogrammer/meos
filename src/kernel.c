#include "tty.h"

void _start_kernel(void) {
	const char msg[] = "loading kernel 6789 123456789\n123456789 123456789 123456789\n\t123456789 123456789 123456789 123456789 123456789";
	
	/* clear the VGA buffer */
	cls();

	/* write our welcome message... */
	puts(msg);
}
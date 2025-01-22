#include "tty.h"
#include "pic.h"

void _start_kernel(void) {
	const char msg[] = "loading kernel 6789 123456789\n123456789 123456789 123456789\n\t123456789 123456789 123456789 123456789 123456789";
	
	enable_apic();

	/* set the default color */
	set_color( LT_GRAY, BLACK );

	/* clear the VGA buffer */
	cls();

	/* write our welcome message... */
	puts( msg );
	putch( '\n' );
	putch( '\n' );

	set_color( YELLOW, BLUE );
	puts( "I (should) look like an old IBM PC XT\n" );
}
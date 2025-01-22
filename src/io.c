/* Reference: http://wiki.osdev.org/Inline_Assembly/Examples#I/O_access */
// Not sure if QW (64-bit) values are done (or used?)
#include "io.h"

/* out{b,w,l --,qw-- } = output a b=8bit, w=16bit, l=32bit, -qw=64bit- to the specified port */
void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
/*
void outw(uint16_t port, uint16_t val) {
	__asm__ volatile ( "outw %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
void outl(uint16_t port, uint32_t val) {
	__asm__ volatile ( "outl %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}
*/
// static inline void outqw(uint16_t port, uint64_t val) {
// 	__asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
// }

/* in{b,w,l --,qw-- } = same as out, but read the value instead of write it */
uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ( "inb %w1, %b0"
	                   : "=a"(ret)
	                   : "Nd"(port)
	                   : "memory");
    return ret;
}
/*
uint16_t inw(uint16_t port) {
	uint16_t ret;
	__asm__ volatile ( "inw %w1, %b0"
	                   : "=a"(ret)
	                   : "Nd"(port)
	                   : "memory");
    return ret;
}
uint32_t inl(uint16_t port);
// static inline uint64_t inqw(uint16_t port);
*/

/* wait a very small amount of time (1-4ms) */
void io_wait(void) {
	outb(0x80, 0);
}
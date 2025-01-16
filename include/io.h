/* Reference: http://wiki.osdev.org/Inline_Assembly/Examples#I/O_access */
#pragma once

#include "types.h"

/* out{b,w,l --,qw--} = output a b=8bit, w=16bit, l=32bit --, qw=64bit-- to the specified port */
static void outb(uint16_t port, uint8_t val);
static void outw(uint16_t port, uint16_t val);
static void outl(uint16_t port, uint32_t val);
//static void outqw(uint16_t port, uint64_t val);

/* in{b,w,l --,qw--} = same as out, but read the value instead of write it */
static uint8_t inb(uint16_t port);
static uint16_t inw(uint16_t port);
static uint32_t inl(uint16_t port);
//static uint64_t inqw(uint16_t port);

/* wait a very small amount of time (1-4ms) */
static void io_wait(void);
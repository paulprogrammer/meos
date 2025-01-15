#pragma once

typedef long size_t;

/* zero out the memory starting at `start` for `length` bytes */
extern int bzero(volatile void *start, size_t length);
#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define PANIC(msg) panic(msg, __FILE__, __LINE__);

void panic(const char *msg, const char *file);

#endif

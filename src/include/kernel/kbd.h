#ifndef _KBD_H
#define _KBD_H

#include <kernel/portio.h>
#include <kernel/irq.h>
#include <kernel/tty.h>
#include <kernel/util/log.h>

#define K_F1 0x3B
#define K_F2 0x3C
#define K_F3 0x3D
#define K_F4 0x3E
#define K_F5 0x3F
#define K_F6 0x40
#define K_F7 0x41
#define K_F8 0x42
#define K_F9 0x43
#define K_F10 0x44

#define K_CTRL 0x01
#define K_ALT 0x02
#define K_SHIFT 0x04
#define K_CAPSLOCK 0x08

void keyboard_handler(struct regs *r);
void keyboard_install();

#endif

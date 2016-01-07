#ifndef _PORTIO_H
#define _PORTIO_H

#include <stdint.h>

struct regs {
  uint32_t gs, fs, es, ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
  uint32_t int_no, err_code;
  uint32_t eip, cs, eflags, useresp, ss;
};

void outportb(uint16_t _port, uint8_t _data);
uint8_t inportb(uint16_t port);

uint16_t inportw(uint16_t port);
void outportw(uint16_t port, uint16_t value);

uint32_t inportl(uint16_t port);
void outportl(uint16_t port, uint32_t value);

#endif

#include <kernel/portio.h>

uint8_t inportb(uint16_t port) {
  uint8_t value;
  asm volatile("inb %%dx, %%al":"=a" (value):"d" (port));
  return value;
}

uint16_t inportw(uint16_t port) {
  uint16_t value;
  asm volatile("inw %%dx, %%ax":"=a" (value):"d" (port));
  return value;
}

uint32_t inportl(uint16_t port) {
  uint32_t value;
  asm volatile("in %%dx, %%eax":"=a" (value):"d" (port));
  return value;
}

void outportb(uint16_t port, uint8_t value) {
  asm volatile("outb %%al, %%dx"::"d" (port), "a" (value));
}

void outportw(uint16_t port, uint16_t value) {
  asm volatile("outw %%ax, %%dx"::"d" (port), "a" (value));
}

void outportl(uint16_t port, uint32_t value) {
  asm volatile("out %%eax, %%dx"::"d" (port), "a" (value));
}

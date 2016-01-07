#ifndef GDT_H
#define GDT_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernel/tty.h>

// Define a GDT entry
struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_middle;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high;
} __attribute__((packed));

// Special pointer which includes the limit
// the max bytes taken up by the GDT, minus 1.
struct gdt_ptr {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr _gp;

// Used in boot/gdt.asm for reloading
// the new segment registers
extern void _gdt_flush();

void gdt_set_gate(uint32_t num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);

void gdt_install();

#endif

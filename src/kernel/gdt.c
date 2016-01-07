#include <kernel/gdt.h>
#include <kernel/util/log.h>

// Setup descriptor in the Global Descriptor Table
void gdt_set_gate(uint32_t num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran) {
  // Setup descriptor base address
  gdt[num].base_low = (base & 0xFFFF);
  gdt[num].base_middle = (base >> 16) & 0xFF;
  gdt[num].base_high = (base >> 24) & 0xFF;

  // Setup descriptor limits
  gdt[num].limit_low = (limit & 0xFFFF);
  gdt[num].granularity = ((limit >> 16) & 0x0F);

  // Setup granularity and access flags
  gdt[num].granularity |= (gran & 0xF0);
  gdt[num].access = access;
}

/* Called by main, sets up the special GDT pointer,
 * sets up the first 3 entries in our GDT, and then
 * calls gdt_flush() in our assembler file in order
 * to tell the processor where the new GDT is and update
 * the segment registers
 */
void gdt_install() {
  kernel_log("os", "Installing GDT\n");
  _gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
  _gp.base = (uint32_t) &gdt;

  // NULL descriptor
  gdt_set_gate(0, 0, 0, 0, 0);

  // Code Segment
  gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  // Data Segment
  gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  // Flush out the old GDT and install the new changes!
  _gdt_flush();
}

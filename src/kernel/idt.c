#include <string.h>

#include <kernel/idt.h>
#include <kernel/tty.h>
#include <kernel/util/log.h>

struct idt_entry idt[256];
struct idt_ptr _idtp;

// Sets an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
  idt[num].base_lo = base & 0xFFFF;
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].always0 = 0;
  idt[num].sel = sel;
  idt[num].flags = flags;
}

// Installs the IDT
void idt_install() {
  kernel_log("os", "Installing IDT\n");

  // Sets up the special IDT pointer
  _idtp.limit = (sizeof(struct idt_entry) * 256 -1);
  _idtp.base = (uint32_t) &idt;

  // Zero out the IDT
  memset(&idt, 0, sizeof(struct idt_entry) * 256);

  uint32_t check = idt[0].base_lo;

  if (!check) {
    kernel_log("os", "Cleared old IDT successfully\n");
  }

  _idt_load();
}

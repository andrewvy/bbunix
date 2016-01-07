#include <kernel/kbd.h>

unsigned char kdbus[128] = {
  0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
  '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e',
  'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
  0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
  ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b',
  'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0,
  0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_install() {
  kernel_log("os", "Installing keyboard\n");
  irq_install_handler(1, keyboard_handler);
}

void keyboard_handler(struct regs *r) {
  (void) r;

  unsigned char scancode;
  scancode = inportb(0x60);

  if (scancode & 0x80) {
    // Top bit of the byte is read means
    // a key has been released.
  } else {
    terminal_kbdinput(kdbus[scancode]);
  }
}

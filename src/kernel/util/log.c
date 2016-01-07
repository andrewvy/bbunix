#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/util/log.h>

void kernel_log(const char *prefix, const char *data) {
  uint8_t last_color = terminal_getcolor();
  uint8_t highlight_color = make_color(COLOR_GREEN, COLOR_WHITE);

  terminal_putchar('[');
  terminal_setcolor(highlight_color);
  terminal_writestring(prefix);
  terminal_setcolor(last_color);
  terminal_writestring("] ");
  terminal_writestring(data);
}

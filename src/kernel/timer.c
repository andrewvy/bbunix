#include <kernel/timer.h>

// Tracks how many ticks the system has been running for
int timer_ticks = 0;

void timer_phase(int hz) {
  int divisor = 1193180 / hz;
  outportb(0x43, 0x36);
  outportb(0x40, divisor & 0xFF);
  outportb(0x40, divisor >> 8);
}

void timer_handler(struct regs *r) {
  (void) r;
  timer_ticks++;
}

void timer_install() {
  kernel_log("os", "Installing timer\n");

  timer_phase(100);
  irq_install_handler(0, timer_handler);
}

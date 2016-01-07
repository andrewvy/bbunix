#include <kernel/portio.h>
#include <kernel/tty.h>
#include <kernel/fault.h>
#include <kernel/irq.h>
#include <kernel/util/log.h>

void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();

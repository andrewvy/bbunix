#include <stdio.h>

#if defined(__is_os_kernel)
#include <kernel/tty.h>
#endif

int putchar(int ic) {
#if defined(__is_os_kernel)
  char c = (char) ic;
  terminal_write(&c, sizeof(c));
#else
  // TODO: implement write system call.
#endif
  return ic;
}

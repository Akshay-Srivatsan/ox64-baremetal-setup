#include "lib.h"

void kmain(void) {
  uart_init(115200);
  while (true) {
    uart_puts("Hello, RISC-V!\n");
  }
}

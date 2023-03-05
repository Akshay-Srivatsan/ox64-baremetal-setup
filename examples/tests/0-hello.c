#include "lib.h"

void kmain(void) {
  // Pins 14 and 15 are already configured for UART output by the bootloader,
  // and the UART is already set up.
  uart_puts("Hello, World!\n");
}

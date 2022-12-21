#include <stdbool.h>
#include <stdint.h>

// page 314-315 of the bl808 reference manual
#define UART0_FIFO_WDATA 0x2000a088

extern void PUT32(uint32_t addr, uint32_t value);
extern uint32_t GET32(uint32_t addr);
extern void put32(volatile uint32_t *addr, uint32_t value);
extern uint32_t get32(volatile uint32_t *addr);

void uart_putc(char c) { PUT32(UART0_FIFO_WDATA, c); }

void uart_puts(const char *c) {
  while (*c) {
    uart_putc(*c++);
  }
}

void kmain(void) {
  // Pins 14 and 15 are already configured for UART output by the bootloader,
  // and the UART is already set up.
  uart_puts("Hello, World!\n");
}

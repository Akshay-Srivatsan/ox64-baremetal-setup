#include "uart.h"
#include "memory.h"

// from the bl808 reference manual
#define UART0_BASE 0x2000a000
#define UART_TX_CFG 0x000
#define UART_BIT_PRD 0x008
#define UART_FIFO_CFG0 0x084
#define UART_FIFO_WDATA 0x088
#define UART_CLOCK 40000000UL

bool uart_can_putc(void) {
  return (GET32(UART0_BASE + UART_FIFO_CFG0) & 0x3f) != 0;
}

void uart_putc(char c) {
  while (!uart_can_putc())
    ;
  PUT32(UART0_BASE + UART_FIFO_WDATA, c);
}

void uart_puthex(uint32_t x) {
  const char *data = "0123456789abcdef";
  uart_putc('0');
  uart_putc('x');
  for (int i = 7; i >= 0; i--) {
    uart_putc(data[(x >> (i * 4)) & 0xf]);
  }
  uart_putc('\n');
}

void uart_puts(const char *c) {
  while (*c) {
    uart_putc(*c++);
  }
}

void uart_init(unsigned baud) {
  // p302
  uint32_t val = 0;
  val |= 1 << 0;  // enable
  val |= 1 << 2;  // freerun mode
  val |= 7 << 8;  // 8 data bits
  val |= 2 << 11; // 1 stop bit
  PUT32(UART0_BASE + UART_TX_CFG, val);

  uint32_t bit_period_value =
      (2 * UART_CLOCK / baud) - 1; // for some reason the uart clock seems to be
                                   // off by a factor of 2 from what we expect
  val = bit_period_value << 16 | bit_period_value;
  PUT32(UART0_BASE + UART_BIT_PRD, val);
}

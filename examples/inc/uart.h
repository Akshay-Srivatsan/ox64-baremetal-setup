#pragma once
#include <stdbool.h>
#include <stdint.h>

bool uart_can_putc(void);
void uart_putc(char c);
void uart_puts(const char *c);
void uart_init(unsigned baud);

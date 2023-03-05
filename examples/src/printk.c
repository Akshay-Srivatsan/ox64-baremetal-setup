#include "printk.h"
#include "uart.h"

void printk(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintk(format, args);
  va_end(args);
}

static void put_hex(unsigned x, bool seen_first) {
  const char *data = "0123456789abcdef";
  for (int i = 7; i >= 0; i--) {
    int nibble = (x >> (i * 4)) & 0xf;
    if (seen_first || nibble) {
      uart_putc(data[nibble]);
      seen_first = true;
    }
  }
  if (!seen_first) {
    uart_putc('0');
  }
}

static void put_bool(unsigned x) {
  bool seen_first = false;
  for (int i = 31; i >= 0; i--) {
    bool bit = (x >> i) & 0b1;
    seen_first |= bit;
    if (seen_first) {
      uart_putc(bit ? '1' : '0');
    }
  }
  if (!seen_first) {
    uart_putc('0');
  }
}

static void put_uint_helper(unsigned x, bool nested) {
  if (x == 0 && nested) {
    return;
  }
  put_uint_helper(x / 10, true);
  const char *data = "0123456789";
  uart_putc(data[x % 10]);
}

static void put_int(unsigned x, bool sign) {
  if (sign && (int)x < 0) {
    uart_putc('-');
    x = -x;
  }
  put_uint_helper(x, false);
}

void vprintk(const char *format, va_list arg) {
  const char *s = format;
  while (*s != 0) {
    char c = *s++;
    char d = *s;
    if (c != '%' || d == 0) {
      uart_putc(c);
      continue;
    }
    s++;
    switch (d) {
      case 'x':
        put_hex(va_arg(arg, unsigned), false);
        break;
      case 'd':
        put_int(va_arg(arg, int), true);
        break;
      case 'u':
        put_int(va_arg(arg, unsigned), false);
        break;
      case 'c':
        uart_putc(va_arg(arg, int));
        break;
      case 's':
        uart_puts(va_arg(arg, char *));
        break;
      case 'p':
        uart_puts("0x");
        put_hex(va_arg(arg, unsigned), true);
        break;
      case 'b':
        uart_puts("0b");
        put_bool(va_arg(arg, unsigned));
        break;
      default:
        uart_putc(c);
        s--;
        break;
    }
  }
}
